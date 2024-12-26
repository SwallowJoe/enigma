# GLSL 着色器

GLSL 着色器包括：

- 顶点着色器（Vertex Shaders）: 处理每个顶点的位置和属性
- 片段着色器（Fragment Shaders）: 处理每个像素的颜色
- 几何着色器（Geometry Shaders）: 处理整个图元并生成新的图元
- 曲面着色器（Tessellation Shaders）: 控制细分曲面的细节程度
- 计算着色器（Compute Shaders）: 执行通用计算任务

这些着色器共同用于渲染管线的不同阶段。

## 顶点着色器

顶点着色器用于处理每个顶点的位置和属性（如位置、颜色、纹理坐标、法线等）并将其转换为裁剪空间坐标; 定点着色器可以执行变换、光照计算等操作。

顶点着色器的输入通常来自于顶点缓冲对象（VBO），这些输入包括顶点位置、法线、纹理坐标等。

> 顶点着色器的输出会传递给片段着色器

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

// 输出到片段着色器的变量
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

## 片段着色器

片段着色器用于处理每个像素的颜色并将其转换为最终颜色。片段着色器可以执行光照、纹理采样等操作。

**片段着色器的输入是从顶点着色器传递过来的插值后的数据。**

> 片段着色器的输出是最终的颜色值。

| 输入 | 输出 |
| --- | --- |
| 顶点颜色（vec3） | 最终颜色（vec4） |
| 顶点法线（vec3） |  - |
| 顶点纹理坐标（vec2） | - |

```glsl
#version 330 core

// 输入来自顶点着色器的变量
in vec3 FragColor;
in vec3 Normal;
in vec2 TexCoord;

// 输出颜色
out vec4 color;

// 纹理
uniform sampler2D texture1;

void main()
{
    // 简单的纹理采样
    color = texture(texture1, TexCoord);
}
```

## 几何着色器

几何着色器（Geometry Shaders）是可选的着色器阶段，位于顶点着色器和片段着色器之间。它可以处理整个图元（如点、线、三角形），并生成新的图元。

> 几何着色器的输入是顶点着色器的输出，输出可以是零个或多个图元。

几何着色器的工作原理

1. 输入：几何着色器接收一个完整的图元（如一个三角形的三个顶点）。
2. 处理：几何着色器可以对输入的图元进行操作，如生成新的顶点、改变图元形状等。
3. 输出：几何着色器输出新的图元，这些图元将传递给片段着色器。

示例代码

以下是一个简单的几何着色器示例，它将每个输入三角形扩展为一个四边形：

```glsl
#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 4) out;

// 输入来自顶点着色器的变量
in vec3 FragPos[];
in vec3 Normal[];
in vec2 TexCoord[];

// 输出传递给片段着色器的变量
out vec3 gFragPos;
out vec3 gNormal;
out vec2 gTexCoord;

void main()
{
    for (int i = 0; i < 3; ++i)
    {
        gFragPos = FragPos[i];
        gNormal = Normal[i];
        gTexCoord = TexCoord[i];
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }

    // 生成第四个顶点
    gFragPos = FragPos[0];
    gNormal = Normal[0];
    gTexCoord = TexCoord[0];
    gl_Position = gl_in[0].gl_Position + vec4(0.1, 0.1, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}
```

总结：

1. 几何着色器输入：来自顶点着色器的图元（如点、线、三角形）。
2. 几何着色器处理：可以生成新的顶点和图元。
3. 几何着色器输出：新的图元传递给片段着色器。

几何着色器在需要动态生成或修改图元的情况下非常有用，如粒子系统、曲面细分等。

## 曲面着色器

曲面着色器（Tessellation Shaders）用于控制细分曲面的细节程度，它包含两个阶段：

1. 曲面细分控制着色器（Tessellation Control Shader，TCS）：控制细分曲面的细节程度。
2. 曲面细分评估着色器（Tessellation Evaluation Shader，TES）：根据细分曲面的细节生成新的顶点。

> 曲面着色器的输入是顶点着色器的输出，输出是新的顶点。

曲面着色器的工作原理：

1. 输入：曲面着色器接收一个或多个输入顶点。
2. 控制：TCS 控制曲面的细节程度，确定细分曲面的顶点数量。
3. 评估：TES 根据细分曲面的细节生成新的顶点。
4. 输出：输出新的顶点，这些顶点将传递给片段着色器。

示例代码

以下是一个简单的曲面着色器示例，它细分一个三角形：

```glsl
#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

// 输入来自顶点着色器的变量
in vec3 FragPos[];
in vec3 Normal[];
in vec2 TexCoord[];

// 输出传递给片段着色器的变量
out vec3 gFragPos;
out vec3 gNormal;
out vec2 gTexCoord;

void main()
{
    for (int i = 0; i < 3; ++i)
    {
        gFragPos = FragPos[i];
        gNormal = Normal[i];
        gTexCoord = TexCoord[i];
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }

    EndPrimitive();
}
```

曲面着色器在需要动态生成或修改曲面的情况下非常有用，如地形细分、水波模拟等。

## 计算着色器

计算着色器（Compute Shaders）是一种独立的着色器阶段，它可以执行通用计算任务，如物理模拟、图像处理、数据处理等。

计算着色器没有输入和输出，它通过调度组（dispatch groups）来执行计算任务。

计算着色器的工作原理：

1. 输入：计算着色器没有输入。
2. 计算：计算着色器执行计算任务。
3. 输出：计算着色器没有输出。

示例代码

以下是一个简单的计算着色器示例，它计算一个数组的和：

```glsl
#version 430 core

layout (local_size_x = 64) in;

// 输入和输出缓冲区
layout (std430, binding = 0) buffer DataIn
{
    float dataIn[];
};

layout (std430, binding = 1) buffer DataOut
{
    float dataOut[];
};

void main()
{
    uint index = gl_GlobalInvocationID.x;
    dataOut[index] = dataIn[index] + 1.0f;
}
```

计算着色器在需要并行计算或通用计算任务的情况下非常有用，如物理模拟、图像处理、数据处理等。
