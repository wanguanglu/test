/*
 * Copyright 1993-2015 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 *
 */

// OpenGL Graphics includes
#include <GL/glew.h>
#if defined(__APPLE__) || defined(MACOSX)
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#include <GLUT/glut.h>
#ifndef glutCloseFunc
#define glutCloseFunc glutWMCloseFunc
#endif
#else
#include <GL/freeglut.h>
#endif

// Includes
#include <helper_functions.h>  // helper functions for timing, string parsing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cuda_gl_interop.h>  // CUDA OpenGL interop
#include <cuda_runtime.h>     // CUDA Runtime

#include <helper_cuda.h>  // includes for CUDA initialization and error checking
#include <helper_cuda_gl.h>

#include "FunctionPointers_kernels.h"

#define EXIT_WAIVED 2
#define MIN_RUNTIME_VERSION 3010
#define MIN_COMPUTE_VERSION 0x20

//
// Cuda example code that implements the Sobel edge detection
// filter. This code works for 8-bit monochrome images.
//
// Use the '-' and '=' keys to change the scale factor.
//
// Other keys:
// I: display image
// T: display Sobel Edge Detection (computed solely with texture)
// S: display Sobel Edge Detection (computed with texture and shared memory)

void cleanup(void);
void initializeData(char* file);

#define MAX_EPSILON_ERROR 5.0f

static const char* sSDKsample = "CUDA Function Pointers (SobelFilter)";

const char* filterMode[] = {"No Filtering", "Sobel Texture",
                            "Sobel SMEM+Texture", NULL};

static int wWidth = 512;   // Window width
static int wHeight = 512;  // Window height
static int imWidth = 0;    // Image width
static int imHeight = 0;   // Image height
static int blockOp = 0;
static int pointOp = 1;

// Code to handle Auto verification
const int frameCheckNumber = 4;
int fpsCount = 0;  // FPS count for averaging
int fpsLimit = 8;  // FPS limit for sampling
unsigned int frameCount = 0;
StopWatchInterface* timer = NULL;
unsigned int g_Bpp;

int g_TotalErrors = 0;

int* pArgc = NULL;
char** pArgv = NULL;

bool g_bQAReadback = false;

// Display Data
static GLuint pbo_buffer = 0;  // Front and back CA buffers
struct cudaGraphicsResource*
    cuda_pbo_resource;  // CUDA Graphics Resource (to transfer PBO)

static GLuint texid = 0;       // Texture for display
unsigned char* pixels = NULL;  // Image pixel data on the host
float imageScale = 1.f;        // Image exposure
enum SobelDisplayMode g_SobelDisplayMode;

#define OFFSET(i) ((char*)NULL + (i))
#define MAX(a, b) ((a > b) ? a : b)
#define REFRESH_DELAY 10  // ms

void computeFPS() {
  frameCount++;
  fpsCount++;

  if (fpsCount == fpsLimit) {
    char fps[256];
    float ifps = 1.f / (sdkGetAverageTimerValue(&timer) / 1000.f);
    sprintf(fps, "FunctionPointers [CUDA Edge Detection] (%s): %3.1f fps",
            filterMode[g_SobelDisplayMode], ifps);

    glutSetWindowTitle(fps);
    fpsCount = 0;

    fpsLimit = (int)MAX(ifps, 1.f);
    sdkResetTimer(&timer);
  }
}

// This is the normal display path
void display(void) {
  sdkStartTimer(&timer);

  // Sobel operation
  Pixel* data = NULL;

  // map PBO to get CUDA device pointer
  checkCudaErrors(cudaGraphicsMapResources(1, &cuda_pbo_resource, 0));
  size_t num_bytes;
  checkCudaErrors(cudaGraphicsResourceGetMappedPointer(
      (void**)&data, &num_bytes, cuda_pbo_resource));
  // printf("CUDA mapped PBO: May access %ld bytes\n", num_bytes);

  sobelFilter(data, imWidth, imHeight, g_SobelDisplayMode, imageScale, blockOp,
              pointOp);
  checkCudaErrors(cudaGraphicsUnmapResources(1, &cuda_pbo_resource, 0));

  glClear(GL_COLOR_BUFFER_BIT);

  glBindTexture(GL_TEXTURE_2D, texid);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo_buffer);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, imWidth, imHeight, GL_LUMINANCE,
                  GL_UNSIGNED_BYTE, OFFSET(0));
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glBegin(GL_QUADS);
  glVertex2f(0, 0);
  glTexCoord2f(0, 0);
  glVertex2f(0, 1);
  glTexCoord2f(1, 0);
  glVertex2f(1, 1);
  glTexCoord2f(1, 1);
  glVertex2f(1, 0);
  glTexCoord2f(0, 1);
  glEnd();
  glBindTexture(GL_TEXTURE_2D, 0);

  glutSwapBuffers();

  sdkStopTimer(&timer);
  computeFPS();
}

void timerEvent(int value) {
  if (glutGetWindow()) {
    glutPostRedisplay();
    glutTimerFunc(REFRESH_DELAY, timerEvent, 0);
  }
}

void keyboard(unsigned char key, int /*x*/, int /*y*/) {
  char temp[256];

  switch (key) {
    case 27:
#if defined(__APPLE__) || defined(MACOSX)
      exit(EXIT_SUCCESS);
#else
      glutDestroyWindow(glutGetWindow());
      return;
#endif
      break;

    case '-':
      imageScale -= 0.1f;
      printf("brightness = %4.2f\n", imageScale);
      break;

    case '=':
      imageScale += 0.1f;
      printf("brightness = %4.2f\n", imageScale);
      break;

    case 'i':
    case 'I':
      g_SobelDisplayMode = SOBELDISPLAY_IMAGE;
      sprintf(temp, "Function Pointers [CUDA Edge Detection] (%s)",
              filterMode[g_SobelDisplayMode]);
      glutSetWindowTitle(temp);
      break;

    case 's':
    case 'S':
      g_SobelDisplayMode = SOBELDISPLAY_SOBELSHARED;
      sprintf(temp, "Function Pointers [CUDA Edge Detection] (%s)",
              filterMode[g_SobelDisplayMode]);
      glutSetWindowTitle(temp);
      break;

    case 't':
    case 'T':
      g_SobelDisplayMode = SOBELDISPLAY_SOBELTEX;
      sprintf(temp, "Function Pointers [CUDA Edge Detection] (%s)",
              filterMode[g_SobelDisplayMode]);
      glutSetWindowTitle(temp);
      break;

    case 'b':
    case 'B':
      blockOp = (blockOp + 1) % LAST_BLOCK_FILTER;
      break;

    case 'p':
    case 'P':
      pointOp = (pointOp + 1) % LAST_POINT_FILTER;
      break;

    default:
      break;
  }
}

void reshape(int x, int y) {
  glViewport(0, 0, x, y);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, 1, 0, 1, 0, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void cleanup(void) {
  cudaGraphicsUnregisterResource(cuda_pbo_resource);

  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
  glDeleteBuffers(1, &pbo_buffer);
  glDeleteTextures(1, &texid);
  deleteTexture();

  sdkDeleteTimer(&timer);

  // cudaDeviceReset causes the driver to clean up all state. While
  // not mandatory in normal operation, it is good practice.  It is also
  // needed to ensure correct operation when the application is being
  // profiled. Calling cudaDeviceReset causes all profile data to be
  // flushed before the application exits
  cudaDeviceReset();
}

void initializeData(char* file) {
  GLint bsize;
  unsigned int w, h;
  size_t file_length = strlen(file);

  if (!strcmp(&file[file_length - 3], "pgm")) {
    if (sdkLoadPGM<unsigned char>(file, &pixels, &w, &h) != true) {
      printf("Failed to load PGM image file: %s\n", file);
      exit(EXIT_FAILURE);
    }

    g_Bpp = 1;
  } else if (!strcmp(&file[file_length - 3], "ppm")) {
    if (sdkLoadPPM4(file, &pixels, &w, &h) != true) {
      printf("Failed to load PPM image file: %s\n", file);
      exit(EXIT_FAILURE);
    }

    g_Bpp = 4;
  } else {
    // cudaDeviceReset causes the driver to clean up all state. While
    // not mandatory in normal operation, it is good practice.  It is also
    // needed to ensure correct operation when the application is being
    // profiled. Calling cudaDeviceReset causes all profile data to be
    // flushed before the application exits
    cudaDeviceReset();
    exit(EXIT_FAILURE);
  }

  imWidth = (int)w;
  imHeight = (int)h;
  setupTexture(imWidth, imHeight, pixels, g_Bpp);

  // copy function pointer tables to host side for later use
  setupFunctionTables();

  memset(pixels, 0x0, g_Bpp * sizeof(Pixel) * imWidth * imHeight);

  if (!g_bQAReadback) {
    // use OpenGL Path
    glGenBuffers(1, &pbo_buffer);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo_buffer);
    glBufferData(GL_PIXEL_UNPACK_BUFFER,
                 g_Bpp * sizeof(Pixel) * imWidth * imHeight, pixels,
                 GL_STREAM_DRAW);

    glGetBufferParameteriv(GL_PIXEL_UNPACK_BUFFER, GL_BUFFER_SIZE, &bsize);

    if ((GLuint)bsize != (g_Bpp * sizeof(Pixel) * imWidth * imHeight)) {
      printf("Buffer object (%d) has incorrect size (%d).\n",
             (unsigned)pbo_buffer, (unsigned)bsize);

      // cudaDeviceReset causes the driver to clean up all state. While
      // not mandatory in normal operation, it is good practice.  It is also
      // needed to ensure correct operation when the application is being
      // profiled. Calling cudaDeviceReset causes all profile data to be
      // flushed before the application exits
      cudaDeviceReset();
      exit(EXIT_FAILURE);
    }

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    // register this buffer object with CUDA
    checkCudaErrors(cudaGraphicsGLRegisterBuffer(
        &cuda_pbo_resource, pbo_buffer, cudaGraphicsMapFlagsWriteDiscard));

    glGenTextures(1, &texid);
    glBindTexture(GL_TEXTURE_2D, texid);
    glTexImage2D(GL_TEXTURE_2D, 0, ((g_Bpp == 1) ? GL_LUMINANCE : GL_BGRA),
                 imWidth, imHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
  }
}

void loadDefaultImage(char* loc_exec) {
  printf("Reading image: lena.pgm\n");
  const char* image_filename = "lena.pgm";
  char* image_path = sdkFindFilePath(image_filename, loc_exec);

  if (image_path == NULL) {
    printf("Failed to read image file: <%s>\n", image_filename);
    exit(EXIT_FAILURE);
  }

  initializeData(image_path);
  free(image_path);
}

void initGL(int* argc, char** argv) {
  glutInit(argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowSize(wWidth, wHeight);
  glutCreateWindow("Function Pointers [CUDA Edge Detection]n");

  glewInit();

  if (!glewIsSupported("GL_VERSION_1_5 GL_ARB_vertex_buffer_object "
                       "GL_ARB_pixel_buffer_object")) {
    fprintf(stderr, "Error: failed to get minimal extensions for demo\n");
    fprintf(stderr, "This sample requires:\n");
    fprintf(stderr, "  OpenGL version 1.5\n");
    fprintf(stderr, "  GL_ARB_vertex_buffer_object\n");
    fprintf(stderr, "  GL_ARB_pixel_buffer_object\n");

    // cudaDeviceReset causes the driver to clean up all state. While
    // not mandatory in normal operation, it is good practice.  It is also
    // needed to ensure correct operation when the application is being
    // profiled. Calling cudaDeviceReset causes all profile data to be
    // flushed before the application exits
    cudaDeviceReset();
    exit(EXIT_WAIVED);
  }
}

bool checkCUDAProfile(int dev, int min_runtime, int min_compute) {
  int runtimeVersion = 0;

  cudaDeviceProp deviceProp;
  cudaGetDeviceProperties(&deviceProp, dev);

  fprintf(stderr, "\nDevice %d: \"%s\"\n", dev, deviceProp.name);
  cudaRuntimeGetVersion(&runtimeVersion);
  fprintf(stderr, "  CUDA Runtime Version     :\t%d.%d\n",
          runtimeVersion / 1000, (runtimeVersion % 100) / 10);
  fprintf(stderr, "  CUDA Compute Capability  :\t%d.%d\n", deviceProp.major,
          deviceProp.minor);

  if (runtimeVersion >= min_runtime &&
      ((deviceProp.major << 4) + deviceProp.minor) >= min_compute) {
    return true;
  } else {
    return false;
  }
}

int findCapableDevice(int argc, char** argv) {
  int dev;
  int bestDev = -1;

  int deviceCount = 0;
  cudaError_t error_id = cudaGetDeviceCount(&deviceCount);

  if (error_id != cudaSuccess) {
    printf("cudaGetDeviceCount returned %d\n-> %s\n", (int)error_id,
           cudaGetErrorString(error_id));
    exit(EXIT_FAILURE);
  }

  if (deviceCount == 0) {
    fprintf(stderr, "There is no device supporting CUDA.\n");
  } else {
    fprintf(stderr, "Found %d CUDA Capable Device(s).\n", deviceCount);
  }

  for (dev = 0; dev < deviceCount; ++dev) {
    cudaDeviceProp deviceProp;
    cudaGetDeviceProperties(&deviceProp, dev);

    if (checkCUDAProfile(dev, MIN_RUNTIME_VERSION, MIN_COMPUTE_VERSION)) {
      fprintf(stderr, "\nFound CUDA Capable Device %d: \"%s\"\n", dev,
              deviceProp.name);

      if (bestDev == -1) {
        bestDev = dev;
        fprintf(stderr, "Setting active device to %d\n", bestDev);
      }
    }
  }

  if (bestDev == -1) {
    fprintf(stderr,
            "\nNo configuration with available capabilities was found. "
            " Test has been waived.\n");
    fprintf(stderr, "This CUDA Sample has minimum requirements:\n");
    fprintf(stderr, "\tCUDA Compute Capability >= %d.%d is required\n",
            MIN_COMPUTE_VERSION / 16, MIN_COMPUTE_VERSION % 16);
    fprintf(stderr, "\tCUDA Runtime Version    >= %d.%d is required\n",
            MIN_RUNTIME_VERSION / 1000, (MIN_RUNTIME_VERSION % 100) / 10);
  }

  return bestDev;
}

void checkDeviceMeetComputeSpec(int argc, char** argv) {
  int device = 0;
  cudaGetDevice(&device);

  if (checkCUDAProfile(device, MIN_RUNTIME_VERSION, MIN_COMPUTE_VERSION)) {
    fprintf(stderr, "\nCUDA Capable Device %d, meets minimum required specs.\n",
            device);
  } else {
    fprintf(stderr,
            "\nNo configuration with minimum compute capabilities "
            "found.  Exiting...\n");
    fprintf(stderr, "This sample requires:\n");
    fprintf(stderr, "\tCUDA Compute Capability >= %d.%d is required\n",
            MIN_COMPUTE_VERSION / 16, MIN_COMPUTE_VERSION % 16);
    fprintf(stderr, "\tCUDA Runtime Version    >= %d.%d is required\n",
            MIN_RUNTIME_VERSION / 1000, (MIN_RUNTIME_VERSION % 100) / 10);

    // cudaDeviceReset causes the driver to clean up all state. While
    // not mandatory in normal operation, it is good practice.  It is also
    // needed to ensure correct operation when the application is being
    // profiled. Calling cudaDeviceReset causes all profile data to be
    // flushed before the application exits
    cudaDeviceReset();
    exit(EXIT_WAIVED);
  }
}

void runAutoTest(int argc, char* argv[]) {
  printf("[%s] (automated testing w/ readback)\n", sSDKsample);
  int devID = findCudaDevice(argc, (const char**)argv);

  // Ensure that SM 2.0 or higher device is available before running
  checkDeviceMeetComputeSpec(argc, argv);

  loadDefaultImage(argv[0]);

  Pixel* d_result;
  checkCudaErrors(
      cudaMalloc((void**)&d_result, imWidth * imHeight * sizeof(Pixel)));

  char* ref_file = NULL;
  char dump_file[256];

  int mode = 0;
  mode = getCmdLineArgumentInt(argc, (const char**)argv, "mode");
  getCmdLineArgumentString(argc, (const char**)argv, "file", &ref_file);

  switch (mode) {
    case 0:
      g_SobelDisplayMode = SOBELDISPLAY_IMAGE;
      sprintf(dump_file, "lena_orig.pgm");
      break;

    case 1:
      g_SobelDisplayMode = SOBELDISPLAY_SOBELTEX;
      sprintf(dump_file, "lena_tex.pgm");
      break;

    case 2:
      g_SobelDisplayMode = SOBELDISPLAY_SOBELSHARED;
      sprintf(dump_file, "lena_shared.pgm");
      break;

    default:
      printf("Invalid Filter Mode File\n");
      exit(EXIT_FAILURE);
      break;
  }

  printf("AutoTest: %s <%s>\n", sSDKsample, filterMode[g_SobelDisplayMode]);
  sobelFilter(d_result, imWidth, imHeight, g_SobelDisplayMode, imageScale,
              blockOp, pointOp);
  checkCudaErrors(cudaDeviceSynchronize());

  unsigned char* h_result =
      (unsigned char*)malloc(imWidth * imHeight * sizeof(Pixel));
  checkCudaErrors(cudaMemcpy(h_result, d_result,
                             imWidth * imHeight * sizeof(Pixel),
                             cudaMemcpyDeviceToHost));
  sdkSavePGM(dump_file, h_result, imWidth, imHeight);

  if (!sdkComparePGM(dump_file, sdkFindFilePath(ref_file, argv[0]),
                     MAX_EPSILON_ERROR, 0.15f, false)) {
    g_TotalErrors++;
  }

  checkCudaErrors(cudaFree(d_result));
  free(h_result);

  if (g_TotalErrors != 0) {
    printf("Test failed!\n");

    // cudaDeviceReset causes the driver to clean up all state. While
    // not mandatory in normal operation, it is good practice.  It is also
    // needed to ensure correct operation when the application is being
    // profiled. Calling cudaDeviceReset causes all profile data to be
    // flushed before the application exits
    cudaDeviceReset();
    exit(EXIT_FAILURE);
  }

  printf("Test passed!\n");

  // cudaDeviceReset causes the driver to clean up all state. While
  // not mandatory in normal operation, it is good practice.  It is also
  // needed to ensure correct operation when the application is being
  // profiled. Calling cudaDeviceReset causes all profile data to be
  // flushed before the application exits
  cudaDeviceReset();
  exit(EXIT_SUCCESS);
}

int main(int argc, char** argv) {
  pArgc = &argc;
  pArgv = argv;

  printf("%s Starting...\n\n", argv[0]);

  if (checkCmdLineFlag(argc, (const char**)argv, "help")) {
    printf("\nUsage: FunctionPointers (SobelFilter) <options>\n");
    printf("\t\t-mode=n (0=original, 1=texture, 2=smem + texture)\n");
    printf("\t\t-file=ref_orig.pgm (ref_tex.pgm, ref_shared.pgm)\n\n");

    exit(EXIT_WAIVED);
  }

  if (checkCmdLineFlag(argc, (const char**)argv, "file")) {
    g_bQAReadback = true;
    runAutoTest(argc, argv);
  }

  // use command-line specified CUDA device, otherwise use device with highest
  // Gflops/s
  if (checkCmdLineFlag(argc, (const char**)argv, "device")) {
    printf(
        "   This SDK does not explicitly support -device=n when running "
        "with OpenGL.\n");
    printf(
        "   When specifying -device=n (n=0,1,2,....) the sample must not "
        "use OpenGL.\n");
    printf("   See details below to run without OpenGL:\n\n");
    printf(" > %s -device=n\n\n", argv[0]);
    printf("exiting...\n");

    // cudaDeviceReset causes the driver to clean up all state. While
    // not mandatory in normal operation, it is good practice.  It is also
    // needed to ensure correct operation when the application is being
    // profiled. Calling cudaDeviceReset causes all profile data to be
    // flushed before the application exits
    cudaDeviceReset();
    exit(EXIT_WAIVED);
  }

  if (!g_bQAReadback) {
    // First initialize OpenGL context, so we can properly set the GL for CUDA.
    // This is necessary in order to achieve optimal performance with
    // OpenGL/CUDA interop.
    initGL(&argc, argv);

    int dev = findCapableDevice(argc, argv);

    checkDeviceMeetComputeSpec(argc, argv);

    if (dev != -1) {
      cudaGLSetGLDevice(dev);
    } else {
      // cudaDeviceReset causes the driver to clean up all state. While
      // not mandatory in normal operation, it is good practice.  It is also
      // needed to ensure correct operation when the application is being
      // profiled. Calling cudaDeviceReset causes all profile data to be
      // flushed before the application exits
      cudaDeviceReset();
      exit(EXIT_WAIVED);
    }

    sdkCreateTimer(&timer);
    sdkResetTimer(&timer);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);

    loadDefaultImage(argv[0]);

    // If code is not printing the USage, then we execute this path.
    printf("I: display Image (no filtering)\n");
    printf("T: display Sobel Edge Detection (Using Texture)\n");
    printf("S: display Sobel Edge Detection (Using SMEM+Texture)\n");
    printf("Use the '-' and '=' keys to change the brightness.\n");
    printf("b: switch block filter operation (Mean/Sobel)\n");
    printf("p: switch point filter operation (Threshold ON/OFF)\n");
    fflush(stdout);

#if defined(__APPLE__) || defined(MACOSX)
    atexit(cleanup);
#else
    glutCloseFunc(cleanup);
#endif

    glutTimerFunc(REFRESH_DELAY, timerEvent, 0);
    glutMainLoop();
  }
}
