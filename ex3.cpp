#include<bits/stdc++.h>
#include <vector>
#include <unordered_set>
#include <Eigen/Core>
#include "base/reconstruction.h"
#include "base/point3d.h"
#include "base/image.h"
#include "base/camera.h" 


using namespace colmap;

void changecolor(const std::string& path,Reconstruction reconstruction,Eigen::Vector3ub color)  {
  std::ofstream file(path, std::ios::trunc);
  CHECK(file.is_open()) << path;

  // Ensure that we don't loose any precision by storing in text.
  file.precision(17);

  file << "# 3D point list with one line of data per point:" << std::endl;
  file << "#   POINT3D_ID, X, Y, Z, R, G, B, ERROR, "
          "TRACK[] as (IMAGE_ID, POINT2D_IDX)"
       << std::endl;
  file << "# Number of points: " << reconstruction.NumPoints3D()
       << ", mean track length: " << reconstruction.ComputeMeanTrackLength() << std::endl;

  for(const point3D_t& point3Did: reconstruction.Point3DIds())
  {
    Point3D point3D;
    point3D=reconstruction.Points3D().at(point3Did);
    file << point3Did << " ";
    file << point3D.XYZ()(0) << " ";
    file << point3D.XYZ()(1) << " ";
    file << point3D.XYZ()(2) << " ";
    file << static_cast<int>(color(0)) << " ";
    file << static_cast<int>(color(1)) << " ";
    file << static_cast<int>(color(2)) << " ";
    file << point3D.Error() << " ";

    std::ostringstream line;

    for (const auto& track_el : point3D.Track().Elements()) {
      line << track_el.image_id << " ";
      line << track_el.point2D_idx << " ";
    }

    std::string line_string = line.str();
    line_string = line_string.substr(0, line_string.size() - 1);

    file << line_string << std::endl;
  }
}

int main()
{
   int branching=4;
   for(int i=0;i<branching;i++){
      Reconstruction reconstruction1; 
      if(i==0){
          reconstruction1.ReadText("/home/lyzlinux/Desktop/experiment/ex6/split/re1");
          Eigen::Vector3ub color(255,0,0);
          changecolor("/home/lyzlinux/Desktop/experiment/ex6/add/points3D0.txt",reconstruction1,color);
      }     
      if(i==1){
          reconstruction1.ReadText("/home/lyzlinux/Desktop/experiment/ex6/split/re2");
          Eigen::Vector3ub color(0,255,0);
          changecolor("/home/lyzlinux/Desktop/experiment/ex6/add/points3D1.txt",reconstruction1,color);
      }
      if(i==2){
          reconstruction1.ReadText("/home/lyzlinux/Desktop/experiment/ex6/split/re3");
          Eigen::Vector3ub color(255,255,0);
          changecolor("/home/lyzlinux/Desktop/experiment/ex6/add/points3D2.txt",reconstruction1,color);
      }
      if(i==3){
          reconstruction1.ReadText("/home/lyzlinux/Desktop/experiment/ex6/split/re4");
          Eigen::Vector3ub color(0,0,255);
          changecolor("/home/lyzlinux/Desktop/experiment/ex6/add/points3D3.txt",reconstruction1,color);
      }
   }
   return 0;
}