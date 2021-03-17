#include "base/image.h"
#include "base/camera.h"
#include "base/point3d.h"
#include <Eigen/Core>
#include <vector>
#include "util/ply.h"
#include "base/reconstruction.h"
#include <unordered_map>

using namespace colmap;

Eigen::Vector3f imagecenter(const Image& image){
    const Eigen::Matrix<float, 3, 4> inv_proj_matrix =
        image.InverseProjectionMatrix().cast<float>();
    const Eigen::Vector3f pc = inv_proj_matrix.rightCols<1>();
    return pc;
}

int main()
{
    int branching =4;
    std::unordered_map<image_t,Point3D> imageplace;
    for(int i=0;i<branching;i++)
    {
        Reconstruction reconstruction1;
       switch(i)
        {   
            case 0:
            reconstruction1.ReadText("/home/lyzlinux/Desktop/experiment/ex6/split/re1/");
            break;
            case 1:
            reconstruction1.ReadText("/home/lyzlinux/Desktop/experiment/ex6/split/re2/");
            break;
            case 2:
            reconstruction1.ReadText("/home/lyzlinux/Desktop/experiment/ex6/split/re3/");
            break;
            case 3:
            reconstruction1.ReadText("/home/lyzlinux/Desktop/experiment/ex6/split/re4/");
            break;
        }
        int r,g,b;
       if(i==0)
          {
               r=255;
               g=255;
               b=0;
           }
        if(i==1)
           {
               r=0;
               g=255;
               b=0;
           }
        if(i==2)
           {
               r=0;
               g=0;
               b=255;
           }
        if(i==3)
           {
               r=255;
               g=0;
               b=0;
           }
           //std::unordered_map<image_t,Point3D> imageplace;
        for(const auto& image1 : reconstruction1.Images())
        {
            Point3D point3D1;
            Eigen::Vector3f pc;
            pc=imagecenter(image1.second);
            double a,b,c;
            a=pc(0);
            b=pc(1);
            c=pc(2);
            Eigen::Vector3d pc1(a,b,c);
            point3D1.SetXYZ(pc1);
            Eigen::Vector3ub color1(r,g,b);
            point3D1.SetColor(color1);
            imageplace.emplace(image1.first,point3D1);
        }
    }
        std::vector<PlyPoint> ply_points;
        ply_points.reserve(imageplace.size());

        for (const auto& point3D : imageplace) {
            PlyPoint ply_point;
            ply_point.x = point3D.second.X();
            ply_point.y = point3D.second.Y();
            ply_point.z = point3D.second.Z();
            ply_point.r = point3D.second.Color(0);
            ply_point.g = point3D.second.Color(1);
            ply_point.b = point3D.second.Color(2);
            ply_points.push_back(ply_point);
        }
        const bool kWriteNormal = false;
        const bool kWriteRGB = true;
        const std::string& path1="/home/lyzlinux/Desktop/experiment/ply/1.ply";
        // const std::string& path2="/home/lyzlinux/Desktop/experiment/ply/2.ply";
        // const std::string& path3="/home/lyzlinux/Desktop/experiment/ply/3.ply";
        // const std::string& path4="/home/lyzlinux/Desktop/experiment/ply/4.ply";
        // switch(i){
        //     case 0:
            WriteBinaryPlyPoints(path1, ply_points, kWriteNormal, kWriteRGB);
        //     break;
        //     case 1:
        //     WriteBinaryPlyPoints(path1, ply_points, kWriteNormal, kWriteRGB);
        //     break;
        //     case 2:
        //     WriteBinaryPlyPoints(path1, ply_points, kWriteNormal, kWriteRGB);
        //     break;
        //     case 3:
        //     WriteBinaryPlyPoints(path1, ply_points, kWriteNormal, kWriteRGB);
        //     break;
        // }
    //  }
}

