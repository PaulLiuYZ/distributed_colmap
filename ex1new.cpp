#include<bits/stdc++.h>
#include <vector>
#include <unordered_set>
#include "base/reconstruction.h"
#include "base/point3d.h"
#include "base/graph_cut.h"
#include "base/track.h"
#include "base/image.h"
#include "base/camera.h" 
#include  <math.h>
#include <Eigen/Core>
#include <algorithm>


using namespace colmap;

struct childgraph{
                std::unordered_set<point3D_t> childpoint3D_ids;
                std::unordered_set<image_t> childimage_ids;
                std::unordered_set<camera_t> childcamera_ids;

   }; 
void childgraphsetup(std::vector<childgraph> & childgraphvector,std::unordered_map<int, int> labels,int branching,std::unordered_set<point3D_t> points3D1_ids,EIGEN_STL_UMAP(point3D_t, class Point3D) points3D1_,EIGEN_STL_UMAP(image_t, class Image) images1_)
{
      for (const point3D_t& childpoint3Did : points3D1_ids){
            if (labels.count(childpoint3Did)){
                   (childgraphvector.at(labels.at(childpoint3Did))).childpoint3D_ids.emplace(childpoint3Did);
                   for (const TrackElement& trackelement : points3D1_.at(childpoint3Did).Track().Elements()){
                         if(childgraphvector.at(labels.at(childpoint3Did)).childimage_ids.find(trackelement.image_id)==childgraphvector.at(labels.at(childpoint3Did)).childimage_ids.end())
                         {
                                childgraphvector.at(labels.at(childpoint3Did)).childimage_ids.emplace(trackelement.image_id);
                                if(childgraphvector.at(labels.at(childpoint3Did)).childcamera_ids.find(images1_.at(trackelement.image_id).CameraId())==childgraphvector.at(labels.at(childpoint3Did)).childcamera_ids.end())
                                    childgraphvector.at(labels.at(childpoint3Did)).childcamera_ids.emplace(images1_.at(trackelement.image_id).CameraId());
                         }     
                    }
              } 
     }
}

Eigen::Vector3f imagecenter(const Image& image){
    const Eigen::Matrix<float, 3, 4> inv_proj_matrix =
        image.InverseProjectionMatrix().cast<float>();
    const Eigen::Vector3f pc = inv_proj_matrix.rightCols<1>();
    return pc;
}

int distance(Reconstruction reconstruction1, double x1, double y1, double z1, image_t image1)
{
    double dis,x2,y2,z2;
    int distance;
    Eigen::Vector3f pc;
    pc=imagecenter(reconstruction1.Images().at(image1));
    x2=pc(0);
    y2=pc(1);
    z2=pc(2);
    dis=pow(pow(x1 - x2, 2) + pow(y1 - y2, 2)+pow(z1-z2,2), 0.5);
    distance =dis;
    return  distance;
} 

int main()
{
      Reconstruction reconstruction1; 
      reconstruction1.ReadText("/home/lyzlinux/Desktop/experiment/ex6/Data5");
      std::vector<std::pair<int, int>> edges;
      std::vector<int> weights;
      int n =reconstruction1.Point3DIds().size();
      for(const point3D_t& point3Did: reconstruction1.Point3DIds()){
            Point3D point3D1;
            point3D1=reconstruction1.Points3D().at(point3Did);
            double x1=point3D1.X();
            double y1=point3D1.Y();
            double z1=point3D1.Z();
            for(const TrackElement& trackelement : point3D1.Track().Elements()){
                        edges.emplace_back(point3Did,-(trackelement.image_id+1));
                        weights.emplace_back(distance(reconstruction1,x1,y1,z1,trackelement.image_id));
                        std::cout<<n<<std::endl;
                        n--;  
                       }
       }
      int branching = 4;
      int maxweight = *max_element(weights.begin(), weights.end());
      std::vector<int> finalweight;
      for(const auto n:weights)
      {
          finalweight.emplace_back(-n+maxweight);
      }
      const auto labels =
               ComputeNormalizedMinGraphCut(edges, finalweight, branching);

      
      std::vector<childgraph> childgraphvector;
      childgraphvector.resize(branching);
      childgraphsetup(childgraphvector,labels,branching,reconstruction1.Point3DIds(),reconstruction1.Points3D(),reconstruction1.Images());
      Reconstruction reconstruction2;
      for (int i=0;i<branching;i++)
      {
           reconstruction2=reconstruction1;
           Reconstruction reconstruction3;
           childgraph childgraph1 = childgraphvector.at(i);
           std::unordered_set<point3D_t> childpoint3D_ids1 = childgraph1.childpoint3D_ids;
           std::unordered_set<image_t> childimage_ids1 = childgraph1.childimage_ids;
           std::unordered_set<camera_t>childcamera_id1 = childgraph1.childcamera_ids;
           for(const point3D_t& point3Did:reconstruction1.Point3DIds())
           {
                 if(childpoint3D_ids1.find(point3Did)==childpoint3D_ids1.end())
                 reconstruction2.DeletePoint3D(point3Did);
           }
           for(const image_t& imageid : reconstruction1.RegImageIds())
           {
                 if(childimage_ids1.find(imageid)==childimage_ids1.end())
                 reconstruction2.DeRegisterImage(imageid);
           }
           for(const auto& camera_1 : reconstruction1.Cameras())
           {   
                if(childcamera_id1.find(camera_1.first)==childcamera_id1.end())
                {
                  //reconstruction2.DeletsCamera(camera_1.first);
                  reconstruction3.AddCamera(camera_1.second);
                  //reconstruction2.Cameras().erase(camera_1.first);
                }
           }
           if(i==0)
           reconstruction2.WriteText("/home/lyzlinux/Desktop/experiment/ex6new/re1");
           reconstruction3.WriteText("/home/lyzlinux/Desktop/experiment/ex6new/re1/camera");
           if(i==1)
           reconstruction2.WriteText("/home/lyzlinux/Desktop/experiment/ex6new/re2");
           reconstruction3.WriteText("/home/lyzlinux/Desktop/experiment/ex6new/re2/camera");
           if(i==2)
           reconstruction2.WriteText("/home/lyzlinux/Desktop/experiment/ex6new/re3");
           reconstruction3.WriteText("/home/lyzlinux/Desktop/experiment/ex6new/re3/camera");
           if(i==3)
           reconstruction2.WriteText("/home/lyzlinux/Desktop/experiment/ex6new/re4");
           reconstruction3.WriteText("/home/lyzlinux/Desktop/experiment/ex6new/re4/camera");
      }
        return 0;
}
            
           
      