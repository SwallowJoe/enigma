# GLSL 着色器

GLSL 着色器包括：

- 顶点着色器（Vertex Shaders）
- 片元着色器（Fragment Shaders）
- 几何着色器（Geometry Shaders）
- 曲面着色器（Tessellation Shaders）
- 计算着色器（Compute Shaders）

这些着色器共同用于渲染管线的不同阶段。

## 定点着色器

顶点着色器用于处理每个顶点的位置和属性（如位置、颜色、纹理坐标、法线等）并将其转换为裁剪空间坐标; 定点着色器可以执行变换、光照计算等操作。

| 输入 | 输出 |
| --- | --- |
| 顶点位置（vec3） | 裁剪空间坐标（vec4） |
| 顶点颜色（vec3） | 顶点颜色（vec3） |
| 顶点法线（vec3） | 顶点法线（vec3） |
| 顶点纹理坐标（vec2） | 顶点纹理坐标（vec2） |


```glsl
#version 330 core

// 输入顶点属性
layout (location = 0) in vec3 aPos;        // 顶点位置
layout (location = 1) in vec3 aColor;      // 顶点颜色
layout (location = 2) in vec3 aNormal;     // 顶点法线
layout (location = 3) in vec2 aTexCoord;   // 顶点纹理坐标

// 输出到片元着色器的变量
out vec3 FragColor;
out vec3 Normal;
out vec2 TexCoord;

// 变换矩阵
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // 将顶点位置转换为裁剪空间坐标
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // 将顶点颜色传递给片元着色器
    FragColor = aColor;

    // 将顶点法线传递给片元着色器
    Normal = aNormal;

    // 将顶点纹理坐标传递给片元着色器
    TexCoord = aTexCoord;
}
```