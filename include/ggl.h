#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <algorithm>

#include <GL/glm/glm/glm.hpp>
#include <GL/glm/glm/gtc/matrix_transform.hpp>
#include <GL/glm/glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glm/glm/gtx/rotate_vector.hpp>

#include <cyCodeBase/cyVector.h>
#include <cyCodeBase/cyMatrix.h>
#include <cyCodeBase/cyTriMesh.h>
#include <cyCodeBase/cyPoint.h>



#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef cy::Vec2<float> Vec2f;
typedef cy::Vec3<float> Vec3f;
typedef cy::Vec4<float> Vec4f;
typedef cy::Matrix2<float> Mat2f;
typedef cy::Matrix3<float> Mat3f;
typedef cy::Matrix4<float> Mat4f;

#define Pi 3.1415926

using namespace std;
using namespace cy;
