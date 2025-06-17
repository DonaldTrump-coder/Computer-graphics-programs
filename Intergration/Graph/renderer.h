#pragma once
#include "glos.h"
#include <vector>
#include <cmath>

#define PI 3.14159265358979323846
// 颜色结构体
struct Color {
    float r, g, b;

    Color operator*(float scalar) const {
        return { r * scalar, g * scalar, b * scalar };
    }

    Color operator+(const Color& other) const {
        return { r + other.r, g + other.g, b + other.b };
    }

    // 颜色值限制到 [0,1] 范围内
    Color& clamp() {
        r = (std::max)(0.0f, (std::min)(1.0f, r));
        g = (std::max)(0.0f, (std::min)(1.0f, g));
        b = (std::max)(0.0f, (std::min)(1.0f, b));
        return *this;  // 返回当前对象，支持链式调用

    }
};

// 3D 向量结构体
struct Vec3 {
    float x, y, z;

    Vec3 operator-(const Vec3& other) const {
        return { x - other.x, y - other.y, z - other.z };
    }

    Vec3 operator+(const Vec3& other) const {
        return { x + other.x, y + other.y, z + other.z };
    }

    Vec3 operator*(float scalar) const {
        return { x * scalar, y * scalar, z * scalar };
    }

    float dot(const Vec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vec3 normalize() const {
        float len = length();
        return { x / len, y / len, z / len };
    }
};

// 定义椭圆结构
struct EllipseInfo {
    float cx, cy;  // 椭圆中心
    float rx, ry;  // 长半轴和短半轴
};

class Renderer
{
private:
    // 设置光源和观察点
    Vec3 lightPos = { 2.0f, 2.0f, 2.0f };  // 点光源
    Vec3 viewPos = { 0.0f, 0.0f, 5.0f };   // 观察者位置
    Color lightColor = { 1.0f, 1.0f, 1.0f };  // 白色光
    Color ambientColor = { 0.2f, 0.2f, 0.2f }; // 环境光
    //ShadeMODE m_ShadeMode = NoMode; //渲染模式1: Gouraud, 2: Phong, 3: Blinn-Phong



    int shadingMode = 1;  // 
public:
    void Renderer::applyShadingModel(
        const std::vector<std::pair<Vec3, Vec3>>& sphere,  // 传入的球体顶点 & 法向量
        const Vec3& viewPos, const Vec3& lightPos,
        const Color& lightColor, const Color& ambientColor,
        float shininess, int shadingMode,
        std::vector<std::pair<Vec3, Color>>& results  // 输出：存储计算后的位置 & 颜色
    );
    Color Renderer::computeGouraudLighting(
        const Vec3& point, const Vec3& normal,
        const Vec3& lightPos, const Color& lightColor,
        const Color& ambientColor);
    Color computePhongLighting(const Vec3& point, const Vec3& normal, const Vec3& viewPos,
        const Vec3 & lightPos, const Color & lightColor, const Color & ambientColor, float shininess);
    Color computeBlinnPhongLighting(const Vec3& point, const Vec3& normal, const Vec3& viewPos,
        const Vec3& lightPos, const Color& lightColor, const Color& ambientColor, float shininess);
    std::vector<std::pair<Vec3, Vec3>> generateSphere(int stacks, int slices, float radius);
    std::vector<EllipseInfo> generateSphereEllipses(float cx, float cy, float r, int stacks, int slices);
    void TransformToScreen(double x, double y, double z, CPoint& ScreenPoint, double m_width, double m_height);

	};

