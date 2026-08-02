#include "helpers.h"



float* calcQuadVertromfWidth(float x, float y, float w, float h, float r, float g, float b, float a, int* outSize)
{
    //float scaler = 10.0f;
    //Pixel to Ndc conversion
    float nx, ny, nw, nh;
    nx = convertPixelXToNDC(x);
    ny = convertPixelYToNDC(y);
    nw = convertPixelWidthToNDC(w);
    nh = convertPixelHeightToNDC(h);

    float z = 0.0f;
    
    float* vertices = malloc((7*4) * sizeof(float));

    if(vertices == NULL){
        printf("Error allocating memory for vertices");
        return NULL;
    }


   // top-right
    vertices[0] = (nx + nw);
    vertices[1] = ny;
    vertices[2] = z;
    vertices[3] = r; 
    vertices[4] = g;
    vertices[5] = b;
    vertices[6] = a;


    // top-left
    vertices[7] = nx;
    vertices[8] = ny;
    vertices[9] = z;
    vertices[10] = r; 
    vertices[11] = g;
    vertices[12] = b;
    vertices[13] = a;

    // bottom-left
    vertices[14] = nx;
    vertices[15] = (ny - nh);
    vertices[16] = z;
    vertices[17] = r; 
    vertices[18] = g;
    vertices[19] = b;
    vertices[20] = a;

    // bottom-right
    vertices[21] = (nx + nw);
    vertices[22] = (ny - nh);
    vertices[23] = z;
    vertices[24] = r; 
    vertices[25] = g;
    vertices[26] = b;
    vertices[27] = a;

    *outSize = (7*4) * sizeof(float);
    return vertices;
}


float convertPixelXToNDC(float x)
{
    float xpos = (x / WINDOW_WIDTH) * 2.0f - 1;
    return xpos;
}

float convertPixelYToNDC(float y)
{
    float ypos = 1.0f - (y / WINDOW_HEIGHT) * 2.0f;
    return ypos;
}


float convertPixelWidthToNDC(float w)
{
    float nw = (w / WINDOW_WIDTH) * 2.0f;
    return nw;
}

float convertPixelHeightToNDC(float h)
{
    float nh = (h / WINDOW_HEIGHT) * 2.0f;
    return nh;
}