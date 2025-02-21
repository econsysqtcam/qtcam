#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLFunctions>
#include <QQuickFramebufferObject>
#include <QOpenGLShaderProgram>
#include <QMutex>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

class Helper : public QObject, public QQuickFramebufferObject::Renderer, public QOpenGLFunctions
{
    Q_OBJECT
    Q_PROPERTY(int irWidth READ getIrWidth NOTIFY irWidthChanged)
    Q_PROPERTY(int irHeight READ getIrHeight NOTIFY irHeightChanged)
    Q_PROPERTY(int isFrameReceived READ isFramesAvailable NOTIFY framesAvailable)
public:
    Helper(QQuickItem *parent = NULL);
    void setImage(const uchar *data, int width, int height, int isCu83);
    void setUpdateStop(bool updateStp);
    void shaderY8();
    void shaderUYVY();
    void clearBufAndShader();
    void clearShader();
    static int irWidth;
    static int irHeight;
    static bool updateStop;
    static bool isFrameReceived;
    void render() override;

public slots:
    void renderY8();
    void renderUYVY();

signals:
    void irWidthChanged();
    void irHeightChanged();
    void framesAvailable();

private:
    static QOpenGLShaderProgram *mShaderProgram;
    static GLfloat vertices[];
    static unsigned short mIndicesData[];
    int mPositionLoc;
    int mTexCoordLoc;
    GLint samplerLoc;
    GLuint m_vbo;
    GLuint greyTextureId;
    GLuint uyvyTextureId;
    QMutex renderMutex;
    static const uchar* irFrame ;
    static int isIrCu83;
    int getIrWidth() const { return irWidth; }
    int getIrHeight() const { return irHeight; }
    bool isFramesAvailable() const { return isFrameReceived;}
};


class ImageItem: public QQuickFramebufferObject
{
    Q_OBJECT
public:
    Renderer *createRenderer() const override {
            if (renderer == nullptr) {
                renderer = new Helper();
            }
            return renderer;
        }

private:
    mutable	Helper	*renderer = nullptr;
};

#endif // RENDERER_H

