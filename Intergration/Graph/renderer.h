#pragma once
#include "glos.h"
#include <vector>
#include <cmath>

#define PI 3.14159265358979323846
// ��ɫ�ṹ��
struct Color {
    float r, g, b;

    Color operator*(float scalar) const {
        return { r * scalar, g * scalar, b * scalar };
    }

    Color operator+(const Color& other) const {
        return { r + other.r, g + other.g, b + other.b };
    }

    // ��ɫֵ���Ƶ� [0,1] ��Χ��
    Color& clamp() {
        r = (std::max)(0.0f, (std::min)(1.0f, r));
        g = (std::max)(0.0f, (std::min)(1.0f, g));
        b = (std::max)(0.0f, (std::min)(1.0f, b));
        return *this;  // ���ص�ǰ����֧����ʽ����

    }
};

// 3D �����ṹ��
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

// ������Բ�ṹ
struct EllipseInfo {
    float cx, cy;  // ��Բ����
    float rx, ry;  // ������Ͷ̰���
};

class Renderer
{
private:
    // ���ù�Դ�͹۲��
    Vec3 lightPos = { 2.0f, 2.0f, 2.0f };  // ���Դ
    Vec3 viewPos = { 0.0f, 0.0f, 5.0f };   // �۲���λ��
    Color lightColor = { 1.0f, 1.0f, 1.0f };  // ��ɫ��
    Color ambientColor = { 0.2f, 0.2f, 0.2f }; // ������
    //ShadeMODE m_ShadeMode = NoMode; //��Ⱦģʽ1: Gouraud, 2: Phong, 3: Blinn-Phong



    int shadingMode = 1;  // 
public:
    void Renderer::applyShadingModel(
        const std::vector<std::pair<Vec3, Vec3>>& sphere,  // ��������嶥�� & ������
        const Vec3& viewPos, const Vec3& lightPos,
        const Color& lightColor, const Color& ambientColor,
        float shininess, int shadingMode,
        std::vector<std::pair<Vec3, Color>>& results  // ������洢������λ�� & ��ɫ
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

