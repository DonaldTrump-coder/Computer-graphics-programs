#include "pch.h"
#include "renderer.h"

#include <iostream>
#include <vector>
#include <cmath>

#define PI 3.14159265358979323846


// 定义新坐标轴到客户区的映射函数
void Renderer::TransformToScreen(double x, double y, double z, CPoint& ScreenPoint, double m_width, double m_height)
{
    double cx = m_width / 2.0;   // 计算客户区中心 x
    double cy = m_height / 2.0;  // 计算客户区中心 y

    // 角度转弧度
    double radian_45 = PI / 4.0;

    // 坐标变换
    double screen_x = cx + x - (y * sin(radian_45));
    double screen_y = cy + (y * cos(radian_45)) - z;

    ScreenPoint.SetPoint(screen_x, screen_y);
}

// 计算漫反射光照（Lambert 模型）
Color Renderer::computeGouraudLighting(
    const Vec3& point, const Vec3& normal,
    const Vec3& lightPos, const Color& lightColor,
    const Color& ambientColor)
{
    // 计算光源方向
    Vec3 lightDir = (lightPos - point).normalize();

    // 计算漫反射强度
    float diffuseIntensity =(std::max)(0.0f, normal.dot(lightDir));

    // 计算最终颜色（环境光 + 漫反射）
    Color finalColor = ambientColor + (lightColor * diffuseIntensity);

    return finalColor.clamp(); // 确保颜色合法
}

// 计算 Phong 光照
Color Renderer::computePhongLighting(const Vec3& point, const Vec3& normal, const Vec3& viewPos,
    const Vec3& lightPos, const Color& lightColor, const Color& ambientColor, float shininess) {
    Vec3 lightDir = (lightPos - point).normalize();
    Vec3 viewDir = (viewPos - point).normalize();
    Vec3 reflectDir = (normal * (2.0f * normal.dot(lightDir))) - lightDir;

    Color ambient = ambientColor * 0.1f;
    float diffuse = (std::max)(0.0f, normal.dot(lightDir));
    float spec = std::pow((std::max)(0.0f, viewDir.dot(reflectDir)), shininess);
    Color diffuseColor = lightColor * diffuse;
    Color specularColor = lightColor * spec*0.3;
    Color finalColor = ambient + diffuseColor + specularColor;
    return finalColor.clamp(); // 确保颜色合法
}

// 计算 Blinn-Phong 光照
Color Renderer::computeBlinnPhongLighting(const Vec3& point, const Vec3& normal, const Vec3& viewPos,
    const Vec3& lightPos, const Color& lightColor, const Color& ambientColor, float shininess) {
    Vec3 lightDir = (lightPos - point).normalize();
    Vec3 viewDir = (viewPos - point).normalize();
    Vec3 halfVector = (lightDir + viewDir).normalize();

    Color ambient = ambientColor * 0.1f;
    float diffuse = (std::max)(0.0f, normal.dot(lightDir));
    float spec = std::pow((std::max)(0.0f, normal.dot(halfVector)), shininess);

    Color diffuseColor = lightColor * diffuse;
    Color specularColor = lightColor * spec;
    Color finalColor = ambient + diffuseColor + specularColor;

    return finalColor.clamp(); // 确保颜色合法
}

// 生成球体顶点
std::vector<std::pair<Vec3, Vec3>> Renderer::generateSphere(int stacks, int slices, float radius) {
    std::vector<std::pair<Vec3, Vec3>> sphere;
    for (int i = 0; i <= stacks; ++i) {
        float phi = PI * i / stacks;
        for (int j = 0; j <= slices; ++j) {
            float theta = 2.0f * PI * j / slices;

            float x = radius * std::sin(phi) * std::cos(theta);
            float y = radius * std::cos(phi);
            float z = radius * std::sin(phi) * std::sin(theta);
            if (x * x + y * y + z * z == radius * radius && y >= 0)
            {
                Vec3 position = { x, y, z };
                Vec3 normal = position.normalize();
                sphere.push_back({ position, normal });
            }

        }
    }
    return sphere;
}

// 选择光照模型，并返回每个顶点的颜色计算结果
void Renderer::applyShadingModel(
    const std::vector<std::pair<Vec3, Vec3>>& sphere,  // 传入的球体顶点 & 法向量
    const Vec3& viewPos, const Vec3& lightPos,
    const Color& lightColor, const Color& ambientColor,
    float shininess, int shadingMode,
    std::vector<std::pair<Vec3, Color>>& results  // 输出：存储计算后的位置 & 颜色
) {

    results.clear();  // 确保结果存储容器是空的

    for (const auto& vertex : sphere) {
        Vec3 position = vertex.first;   // 获取顶点坐标
        Vec3 normal = vertex.second;    // 获取法向量
        Color finalColor;

        // 选择光照模型
        if (shadingMode == 1) {
            finalColor = computeGouraudLighting(position, normal,lightPos, lightColor, ambientColor);
        }
        else if (shadingMode == 2) {
            finalColor = computePhongLighting(position, normal, viewPos, lightPos, lightColor, ambientColor, shininess);
        }
        else if (shadingMode == 3) {
            finalColor = computeBlinnPhongLighting(position, normal, viewPos, lightPos, lightColor, ambientColor, shininess);
        }

        // 存入结果（顶点位置 & 计算后的颜色）
        results.emplace_back(position, finalColor);
    }
}


// 计算用于表示球体的椭圆
std::vector<EllipseInfo>  Renderer::generateSphereEllipses(float cx, float cy, float r, int stacks, int slices) {
    std::vector<EllipseInfo> ellipses;
    // 纬线：随着 z 变化，计算水平截面椭圆
    //纬线最大圆半径
    float ra = 0.3 * r;
    for (int i = 1; i < stacks; ++i) {
        float theta = PI * i / stacks - PI / 2;  // 纬度角 (-π/2 ~ π/2)
        float rx = r * cos(theta);   // 椭圆的长半轴（X 方向投影）
        float ry = ra * cos(theta);   // 椭圆的短半轴（Z 方向投影）

        float yOffset = r * sin(theta);   // 纬线上点的高度偏移
        float new_cy = cy + yOffset;  // 纬线所在的中心高度
        ellipses.push_back({ cx, new_cy, rx, ry });
    }

    // 经线：所有经线在平面上的投影都是相同的椭圆
    for (int i = 0; i < slices; ++i) {
        float phi = 2 * PI * i / slices;  // 经度角 (0 ~ 2π)
        float rx = r * sin(phi);   // 椭圆的长半轴（X 方向投影）
        float ry = r;   // 椭圆的短半轴（Z 方向投影）
        // 经线的投影是一个相同大小的椭圆
        ellipses.push_back({ cx, cy, rx, ry });
    }
    return ellipses;
}



