/*
 * This file is used to recieve irFrame as uchar* from another file/Class
 * and render it in another window
 * Copyright © 2024  e-con Systems India Pvt. Limited
 * This file is part of Qtcam.
 * Qtcam is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * Qtcam is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Qtcam. If not, see <http://www.gnu.org/licenses/>.
*/

#include "renderer.h"

Helper::Helper(QQuickItem *parent)
{
}
GLfloat Helper::vertices[] = {
    // Positions          // Texture Coords
    -1.0f,  1.0f, 0.0f,  0.0f, 1.0f, // Top-left
    -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, // Bottom-left
     1.0f, -1.0f, 0.0f,  1.0f, 0.0f, // Bottom-right
     1.0f, -1.0f, 0.0f,  1.0f, 0.0f, // Bottom-right
     1.0f,  1.0f, 0.0f,  1.0f, 1.0f, // Top-right
    -1.0f,  1.0f, 0.0f,  0.0f, 1.0f  // Top-left again
};

unsigned short Helper::mIndicesData[] = { 0, 1, 2, 0, 2, 3 };

uchar* Helper::irFrame = nullptr;
int Helper::irWidth = 0;
int Helper::irHeight = 0;
int Helper::isIrCu83 = 0;
bool Helper::isFrameReceived = false;
bool Helper::updateStop = false;
QOpenGLShaderProgram *Helper::mShaderProgram = nullptr;


void Helper::setImage(uchar* image, int width, int height, int totalSize, int isCu83)
{
    renderMutex.lock();
    if(width != irWidth){
        irWidth = width;
        emit irWidthChanged();
    }
    if(height != irHeight){
        irHeight = height;
        emit irHeightChanged();
    }
    if(isIrCu83 != isCu83){
        isIrCu83 = isCu83 ;
    }
    if(!isFrameReceived){
        isFrameReceived = true;
        emit framesAvailable();
    }
    if(image != nullptr){
        memcpy(irFrame, image, totalSize);
    }
    renderMutex.unlock();
}

void Helper::setUpdateStop(bool updateStp){
    updateStop = updateStp;
}

void Helper::render()
{
    if(irFrame != nullptr){
        if (mShaderProgram == nullptr) {
            if(isIrCu83 == 1){ // To apply shader for ir frame in Cu83
                shaderY8();
            }
            else if(isIrCu83 == 0){ // To apply shader for ir frame in 27cug
                shaderUYVY();
            }
        }
        if(isIrCu83 == 1 && !updateStop){ // To render cu83 frames in fbo
            renderY8();
        }
        else if(isIrCu83 == 0  && !updateStop){// To render 27cug frames in fbo
            renderUYVY();
        }
    }
    update();
}

void Helper::clearBufAndShader()
{
    if(mShaderProgram){
        if (mShaderProgram->isLinked()) {
            mShaderProgram->release();
            mShaderProgram->removeAllShaders();
            mShaderProgram = nullptr;
            delete mShaderProgram;
        }
    }
    irFrame = nullptr;
}

void Helper::clearShader(){
    if(mShaderProgram){
        if (mShaderProgram->isLinked()) {
            mShaderProgram->release();
            mShaderProgram->removeAllShaders();

            mShaderProgram = nullptr;
            delete mShaderProgram;
        }
    }
}

void Helper::shaderUYVY()
{
    clearShader();
    if (mShaderProgram != nullptr) {
        delete mShaderProgram;
        mShaderProgram = nullptr;
    }

    if (mShaderProgram == nullptr) {
        initializeOpenGLFunctions();

        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        mShaderProgram = new QOpenGLShaderProgram();

        mShaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex,
            "attribute vec4 a_position;\n"
            "attribute vec2 a_texCoord;\n"
            "varying vec2 v_texCoord;\n"
            "void main()\n"
            "{\n"
            "gl_Position = a_position;\n"
            "v_texCoord = a_texCoord;\n"
            "}\n");

        mShaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment,
            "#ifdef GL_ES\n"
            "precision highp float;\n"
            "#endif\n"

            "varying vec2 v_texCoord;\n"
            "uniform sampler2D uyvy_texture;\n"

            "uniform float texel_width;"
            "uniform float texture_width;"
            "uniform float texture_height;"


            "void main()\n"
            "{\n"
            "float r, g, b, y, u, v;\n"

            "   vec4 luma_chroma;\n"
            "   float xcoord = floor(v_texCoord.x * texture_width);\n"
            "   float ycoord = floor(v_texCoord.y * texture_height);\n"

            //We had put the Y values of each pixel to the R,G,B components by
            //GL_LUMINANCE, that's why we're pulling it from the R component,
            //we could also use G or B

            "if (mod(xcoord, 2.0) == 0.0) {\n"
            "   luma_chroma = texture2D(uyvy_texture, v_texCoord);\n"
            "   y = luma_chroma.g;\n"
            "} else {\n"
            "   luma_chroma = texture2D(uyvy_texture, vec2(v_texCoord.x - texel_width, v_texCoord.y));\n"
            "   y = luma_chroma.a;\n"
            "}\n"
            "u = luma_chroma.r - 0.5;\n"
            "v = luma_chroma.b - 0.5;\n"

            //We had put the U and V values of each pixel to the A and R,G,B
            //components of the texture respectively using GL_LUMINANCE_ALPHA.
            //Since U,V bytes are interspread in the texture, this is probably
            //the fastest way to use them in the shader

            //The numbers are just YUV to RGB conversion constants
            "r = y + 1.13983*v;\n"
            "g = y - 0.39465*u - 0.58060*v;\n"
            "b = y + 2.03211*u;\n"

            "gl_FragColor = vec4(r,g,b,1.0);\n"
            "}\n"
        );

        mShaderProgram->bind();

        mPositionLoc = mShaderProgram->attributeLocation("a_position");
        mTexCoordLoc = mShaderProgram->attributeLocation("a_texCoord");

        mShaderProgram->link();
        //Set up texture

        glGenTextures(1, &uyvyTextureId);
        glBindTexture(GL_TEXTURE_2D, uyvyTextureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        //textureData.resize(300 * 400, 255);
        glBindTexture(GL_TEXTURE_2D, uyvyTextureId);

        //Get the sampler location
        samplerLoc = mShaderProgram->uniformLocation("uyvy_texture");

        glUniform1i(samplerLoc, 0);
    }
}

void Helper::renderUYVY()
{
    mShaderProgram->bind();
    glBindTexture(GL_TEXTURE_2D, uyvyTextureId);

    if (irFrame != nullptr)
    {
        if (renderMutex.try_lock())
        {
            if (irFrame != nullptr)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, irWidth / 2, irHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, irFrame);
            }
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glEnableVertexAttribArray(mPositionLoc);
    glVertexAttribPointer(mPositionLoc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(mTexCoordLoc);
    glVertexAttribPointer(mTexCoordLoc, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    // Bind texture
    glActiveTexture(GL_TEXTURE0);

    // Draw
    glDrawArrays(GL_TRIANGLES, 0, 6);

    renderMutex.unlock();

    // Clean up
    glDisableVertexAttribArray(mPositionLoc);
    glDisableVertexAttribArray(mTexCoordLoc);
    mShaderProgram->release();
}


void Helper::shaderY8() {
    clearShader();
    if (mShaderProgram != nullptr) {
        delete mShaderProgram;
        mShaderProgram = NULL;
    }

    if (mShaderProgram == nullptr) {
        initializeOpenGLFunctions();

        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        mShaderProgram = new QOpenGLShaderProgram();

        mShaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex,
            "attribute vec4 a_position;\n"
            "attribute vec2 a_texCoord;\n"
            "varying vec2 v_texCoord;\n"
            "void main()\n"
            "{\n"
            "gl_Position = a_position;\n"
            "v_texCoord = a_texCoord;\n"
            "}\n");

        mShaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment,
            "#ifdef GL_ES\n"
            "precision highp float;\n"
            "#endif\n"

            "varying vec2 v_texCoord;\n"
            "uniform sampler2D grey_texture;\n"

            "void main()\n"
            "{\n"
            "float y;\n"
            //We had put the Y values of each pixel to the R,G,B components by
            //GL_LUMINANCE, that's why we're pulling it from the R component,
            //we could also use G or B

            "y = texture2D(grey_texture, v_texCoord).r;\n"
            "gl_FragColor = vec4(y,y,y,1.0);\n"
            "}\n");
        mShaderProgram->bind();

        mPositionLoc = mShaderProgram->attributeLocation("a_position");
        mTexCoordLoc = mShaderProgram->attributeLocation("a_texCoord");

        mShaderProgram->link();

        // Set up texture
        glGenTextures(1, &greyTextureId);
        glBindTexture(GL_TEXTURE_2D, greyTextureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Get the sampler location
        samplerLoc = mShaderProgram->uniformLocation("grey_texture");

        glUniform1i(samplerLoc, 0);

    }
}

void Helper::renderY8() {

    mShaderProgram->bind();
    glBindTexture(GL_TEXTURE_2D, greyTextureId);

    if ( irFrame != NULL)
    {
       if (renderMutex.try_lock())
        {
            if (irFrame != nullptr)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, irWidth, irHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, irFrame);
            }
            renderMutex.unlock();
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glEnableVertexAttribArray(mPositionLoc);
    glVertexAttribPointer(mPositionLoc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(mTexCoordLoc);
    glVertexAttribPointer(mTexCoordLoc, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    // Bind texture
    glActiveTexture(GL_TEXTURE0);

    // Draw
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Clean up
    glDisableVertexAttribArray(mPositionLoc);
    glDisableVertexAttribArray(mTexCoordLoc);
    mShaderProgram->release();
}
