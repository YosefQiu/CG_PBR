
## <center>CS 6610 Interactive Comp Graph</center>

-----
### Final Project PBR
### Name: Yongfeng Qiu & Zhongyi Jiang  


### Operating environment
- Windows10
- MacOS Monterey(12.0.1)

### Library
- **GLFW** (GLFW provides a simple API for creating windows, contexts, and surfaces, receiving input and events.)
- **GLAD** (GLAD is a Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator based on the official specs.)
- **ImGui** (ImGui is a bloat-free graphical user interface library for C++.)
- **GLM** (OpenGL Mathematics (GLM) is a header-only C++ mathematics library for graphics software based on the OpenGL Shading Language (GLSL) specifications.
- **stb_image** (Decoding different type images like JPG, PNG)
- **ASSIMP** (A library to import and export various 3d-model-formats including scene-post-processing to generate missing render data.)

### Compiling environment
- CMake   
- In the Win10, I use CMake + VS2019. 
- In the MacOS, I use CMake + Makefile + NeoVim

------

### What I implemented

- **Specular BRDF**
  - **Cook-Torrance**  
    - $F_r(v,l) = \frac{D(h,a)G(v,l,a)F(v,h,F_0)}{4(n \cdot v)(n \cdot l)}$
    - **Normal distribution function** 
      - $D_{GGX}(h,a) = \frac{a^2}{\pi ((n \cdot h)^2(a^2-1)+1)^2}$
    - **Smith geometric shadowing function**
      -  $G(v,l,a)=\frac{2(n \cdot l)}{n \cdot l + \sqrt{a^2 + (1-a^2)(n \cdot l)^2}} \frac{2(n \cdot v)}{n \cdot v + \sqrt{a^2 + (1-a^2)(n \cdot v)^2}}$
    - **Fresnel** 
      - $F_{Schlick}(v,h,F_0, F_{90})=F_0+(F_{90}-F_0)(1-v \cdot h)^5$
  - **Cloth specular BRDF (Ashikhminoo)**
    - $D_{velvet}(v,h,a)=c_{norm}(1+4exp(\frac{-cot^2 \theta_h}{a^2}))$
    - $F_r(v,h,a) = \frac{1}{\pi (1+4a^2)}(1+4 \frac{exp(\frac{-cot^2 \theta_h}{a^2})}{sin^4 \theta_h})$
- **Diffuse BRDF**
  - **Lambertian function**
    -   $F_d(v,l) = \frac{\sigma}{\pi}$
  - **Disney diffuse BRDF (Burley)**
    - $ F_d(v,l) = \frac{\sigma}{\pi}F_{Schlick}(n,l,1,F_{90})F_{Schlick}(n, v, 1,F_{90})$
- **Physicall based camera**
  - Position, Target, WorldUp, Pitch, Yaw
  - Aperture
  - Shutter speed
  - Sensitivity
- **Texture**
  - Procedural Texture
    - noise
  - Image Texture
    - AlbedoMap, NormalMap, RoughnessMap, DisplacementMap, AoMap, MetallicMap
  - Solid Texture
- **Light**
  - Theory light
    - Point Light, Spot Light,  Directional Light
  - Area Light
  - Image Base Light
- **UI**
  - camera
  - light
- **Disney principle **


### What I will implement

- **Light**
  - IBL
  - Area Light
- More detailed Disney rules
- Optimize, modify procedural Texture
- Optimize, modify cloth specular BRDF

### How to use my implementation
- In Windows

1. Use CMake to compile the program
2. Set finalproj as startup item
3. Run the program

- In MacOS

````shell
cd /Project directory/CS6610_Final
rm build && mkdir build
cd build
cmake ..
make 
./Projectfinal
make clean
`````

----

### Result

Here are my result. I also submitted a video showing how I compiled and ran on Windows. It also has the running results of my program. 

**Material**

- Image Texture(AlbedoMap, NormalMap, RoughnessMap, DisplacementMap, AoMap, MetallicMap) 

![img](result/PBR22.PNG)

![img](result/PBR3.PNG)

- Solid Texture(Albedo, SheenColor, Metallic, Roughness, Ao)

![img](result/cloth1.png)

![img](result/cloth2.png)

- Procedural Texture

![img](result/noise2.PNG)

![img](result/noise1.png)

**Physicall based camera**

![img](result/phy_camera.PNG)

**Light**
- Theory Light

![image](result/PBR4.PNG)

- Image base light

![img](result/ibl2.png)

- Area light

![img](result/Arealight.png)



**Disney principle**

![img](result/dis2.PNG)





**References**
[1] Burley B, Studios W D A. Physically-based shading at disney[C]//ACM SIGGRAPH. 2012   
[2] Pharr M, Jakob W, Humphreys G. Physically based rendering: From theory to implementa- tion[M]. Morgan Kaufmann, 2016.   
