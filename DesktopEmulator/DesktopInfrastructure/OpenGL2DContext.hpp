// *****************************************************************************
    // start include guard
    #ifndef OPENGL2DCONTEXT_HPP
    #define OPENGL2DCONTEXT_HPP
    
    // include common Vircon headers
    #include "../../VirconDefinitions/DataStructures.hpp"
    #include "../../VirconDefinitions/Enumerations.hpp"
    
    // include project headers
    #include "EmulatorInterfaces.hpp"
    
    // include SDL2 headers
    #define SDL_MAIN_HANDLED
    #include <SDL2/SDL.h>       // [ SDL2 ] Main header
    
    // this is needed by OpenGL headers under Windows
    #if defined(__WIN32__) || defined(_WIN32) || defined(_WIN64)
      #include <windef.h>
    #endif
    
    // include OpenGL headers
    #include <glad/glad.h>      // [ OpenGL ] GLAD Loader (already includes <GL/gl.h>)
// *****************************************************************************


// =============================================================================
//      2D-SPECIALIZED OPENGL CONTEXT
// =============================================================================


class OpenGL2DContext
{
    public:
        
        // graphical settings
        unsigned WindowWidth;
        unsigned WindowHeight;
        unsigned WindowedZoomFactor;
        bool FullScreen;
        
        // video context objects
        SDL_Window* Window;
        SDL_GLContext OpenGLContext;
        
        // framebuffer object
        GLuint FramebufferID;
        GLuint FBColorTextureID;
        unsigned FramebufferWidth;
        unsigned FramebufferHeight;

        // additional GL objects
        GLuint VAO;
        GLuint VBOPositions;
        GLuint VBOTexCoords;
        GLuint ShaderProgramID;
        
        // positions of shader parameters
        GLuint PositionsLocation;
        GLuint TexCoordsLocation;
        GLuint TextureUnitLocation;
        GLuint MultiplyColorLocation;
        
        // arrays to hold buffer info
        GLfloat QuadPositionCoords[ 8 ];
        GLfloat QuadTextureCoords[ 8 ];
        
        // multiply color
        V32::GPUColor MultiplyColor;
        
        // white texture used to draw solid colors
        GLuint WhiteTextureID;
        
    public:
        
        // instance handling
        OpenGL2DContext();
       ~OpenGL2DContext();
        
        // init functions
        void CreateOpenGLWindow();
        void CreateFramebuffer();
        bool CompileShaderProgram();
        void CreateWhiteTexture();
        void InitRendering();
        
        // release functions
        void Destroy();
        
        // view configuration
        void SetWindowZoom( int ZoomFactor );
        void SetFullScreen();
        
        // framebuffer render functions
        void RenderToScreen();
        void RenderToFramebuffer();
        void DrawFramebufferOnScreen();
        
        // color control functions
        void SetMultiplyColor( V32::GPUColor NewMultiplyColor );
        void SetBlendingMode( V32::IOPortValues BlendingMode );
        
        // render functions
        void DrawTexturedQuad( const V32::GPUQuad& Quad );
        void ClearScreen( V32::GPUColor ClearColor );
};


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************
