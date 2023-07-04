// *****************************************************************************
    // include common Vircon headers
    #include "../../VirconDefinitions/Enumerations.hpp"
    
    // include infrastructure headers
    #include "../DesktopInfrastructure/NumericFunctions.hpp"
    
    // include project headers
    #include "V32GPU.hpp"
    #include "Globals.hpp"
    
    // include C/C++ headers
    #include <cmath>            // [ ANSI C ] Mathematics
    
    // declare used namespaces
    using namespace std;
// *****************************************************************************


namespace V32
{
    // =============================================================================
    //      PORT WRITE FUNCTIONS FOR V32 GPU
    // =============================================================================
    
    
    void WriteGPUCommand( V32GPU& GPU, V32Word Value )
    {
        // now execute the command, if valid
        switch( Value.AsInteger )
        {
            case (int32_t)IOPortValues::GPUCommand_ClearScreen:
                GPU.ClearScreen();
                return;
                
            case (int32_t)IOPortValues::GPUCommand_DrawRegion:
                GPU.DrawRegion( false, false );
                return;
                
            case (int32_t)IOPortValues::GPUCommand_DrawRegionZoomed:
                GPU.DrawRegion( true, false );
                return;
                
            case (int32_t)IOPortValues::GPUCommand_DrawRegionRotated:
                GPU.DrawRegion( false, true );
                return;
                
            case (int32_t)IOPortValues::GPUCommand_DrawRegionRotozoomed:
                GPU.DrawRegion( true, true );
                return;
                
            // (unknown command codes are just ignored)
            default: return;
        }
        
        // do not write the value;
        // it is useless anyway (this port is write-only)
    }
    
    // -----------------------------------------------------------------------------
    
    void WriteGPURemainingPixels( V32GPU& GPU, V32Word Value )
    {
        // (ignore request: this register is read-only)
    }
    
    // -----------------------------------------------------------------------------
    
    void WriteGPUClearColor( V32GPU& GPU, V32Word Value )
    {
        // just write the value
        GPU.ClearColor = Value.AsColor;
    }
    
    // -----------------------------------------------------------------------------
    
    void WriteGPUMultiplyColor( V32GPU& GPU, V32Word Value )
    {
        // first write the value
        GPU.MultiplyColor = Value.AsColor;
        
        // now update the corresponding value for OpenGL shaders
        OpenGL2D.MultiplyColor = Value.AsColor;
    }
    
    // -----------------------------------------------------------------------------
    
    void WriteGPUActiveBlending( V32GPU& GPU, V32Word Value )
    {
        switch( Value.AsInteger )
        {
            case (int32_t)IOPortValues::GPUBlendingMode_Alpha:
            {
                OpenGL2D.SetBlendingMode( IOPortValues::GPUBlendingMode_Alpha );
                GPU.ActiveBlending = Value.AsInteger;
                break;
            }
            
            case (int32_t)IOPortValues::GPUBlendingMode_Add:
            {
                OpenGL2D.SetBlendingMode( IOPortValues::GPUBlendingMode_Add );
                GPU.ActiveBlending = Value.AsInteger;
                break;
            }
            
            case (int32_t)IOPortValues::GPUBlendingMode_Subtract:
            {
                OpenGL2D.SetBlendingMode( IOPortValues::GPUBlendingMode_Subtract );
                GPU.ActiveBlending = Value.AsInteger;
                break;
            }
        }
        
        // unknown blending mode codes are just ignored
        // (the value is not written either)
    }
    
    // -----------------------------------------------------------------------------
    
    void WriteGPUSelectedTexture( V32GPU& GPU, V32Word Value )
    {
        // prevent setting a non-existent texture
        if( Value.AsInteger < -1 || Value.AsInteger >= (int32_t)GPU.CartridgeTextures.size() )
          return;
            
        // write the value
        GPU.SelectedTexture = Value.AsInteger;
        
        // now update the pointed entities
        if( Value.AsInteger == -1 )
        {
            // special case for BIOS texture
            GPU.PointedTexture = &GPU.BiosTexture;
            GPU.PointedRegion = &GPU.PointedTexture->Regions[ GPU.SelectedRegion ];
        }
        else
        {
            // regular cartridge textures
            GPU.PointedTexture = &GPU.CartridgeTextures[ GPU.SelectedTexture ];
            GPU.PointedRegion = &GPU.PointedTexture->Regions[ GPU.SelectedRegion ];
        }
    }
    
    // -----------------------------------------------------------------------------
    
    void WriteGPUSelectedRegion( V32GPU& GPU, V32Word Value )
    {
        // prevent setting a non-existent region
        if( Value.AsInteger < 0 || Value.AsInteger >= Constants::GPURegionsPerTexture )
          return;
        
        // write the value
        GPU.SelectedRegion = Value.AsInteger;
        
        // update pointed entity
        GPU.PointedRegion = &GPU.PointedTexture->Regions[ GPU.SelectedRegion ];
    }
    
    // -----------------------------------------------------------------------------
    
    void WriteGPUDrawingPointX( V32GPU& GPU, V32Word Value )
    {
        // out of range values are accepted, but they are clamped
        Clamp( Value.AsInteger, -1000, Constants::ScreenWidth + 1000 );
        GPU.DrawingPointX = Value.AsInteger;
    }
    
    // -----------------------------------------------------------------------------
    
    void WriteGPUDrawingPointY( V32GPU& GPU, V32Word Value )
    {
        // out of range values are accepted, but they are clamped
        Clamp( Value.AsInteger, -1000, Constants::ScreenHeight + 1000 );
        GPU.DrawingPointY = Value.AsInteger;
    }
    
    // -----------------------------------------------------------------------------
    
    void WriteGPUDrawingScaleX( V32GPU& GPU, V32Word Value )
    {
        // ignore non-numeric values
        if( isnan( Value.AsFloat ) || isinf( Value.AsFloat ) )
          return;
        
        // out of range values are accepted, but they are clamped
        Clamp( Value.AsFloat, -1024, 1024 );
        GPU.DrawingScaleX = Value.AsFloat;
    }
    
    // -----------------------------------------------------------------------------
    
    void WriteGPUDrawingScaleY( V32GPU& GPU, V32Word Value )
    {
        // ignore non-numeric values
        if( isnan( Value.AsFloat ) || isinf( Value.AsFloat ) )
          return;
        
        // out of range values are accepted, but they are clamped
        Clamp( Value.AsFloat, -1024, 1024 );
        GPU.DrawingScaleY = Value.AsFloat;
    }
    
    // -----------------------------------------------------------------------------
    
    void WriteGPUDrawingAngle( V32GPU& GPU, V32Word Value )
    {
        // ignore non-numeric values
        if( isnan( Value.AsFloat ) || isinf( Value.AsFloat ) )
          return;
        
        // out of range values are accepted, but they are clamped
        Clamp( Value.AsFloat, -1024, 1024 );
        GPU.DrawingAngle = Value.AsFloat;
    }
    
    // -----------------------------------------------------------------------------
    
    void WriteGPURegionMinX( V32GPU& GPU, V32Word Value )
    {
        // out of texture values are accepted,
        // but they are clamped to texture limits
        Clamp( Value.AsInteger, 0, Constants::GPUTextureSize-1 );
        GPU.PointedRegion->MinX = Value.AsInteger;
    }
    
    // -----------------------------------------------------------------------------
    
    void WriteGPURegionMinY( V32GPU& GPU, V32Word Value )
    {
        // out of texture values are accepted,
        // but they are clamped to texture limits
        Clamp( Value.AsInteger, 0, Constants::GPUTextureSize-1 );
        GPU.PointedRegion->MinY = Value.AsInteger;
    }
    
    // -----------------------------------------------------------------------------
    
    void WriteGPURegionMaxX( V32GPU& GPU, V32Word Value )
    {
        // out of texture values are accepted,
        // but they are clamped to texture limits
        int32_t ValidX = Value.AsInteger;
        Clamp( ValidX, 0, Constants::GPUTextureSize-1 );
        
        GPU.PointedRegion->MaxX = ValidX;
    }
    
    // -----------------------------------------------------------------------------
    
    void WriteGPURegionMaxY( V32GPU& GPU, V32Word Value )
    {
        // out of texture values are accepted,
        // but they are clamped to texture limits
        Clamp( Value.AsInteger, 0, Constants::GPUTextureSize-1 );
        GPU.PointedRegion->MaxY = Value.AsInteger;
    }
    
    // -----------------------------------------------------------------------------
    
    void WriteGPURegionHotspotX( V32GPU& GPU, V32Word Value )
    {
        // out of texture values are valid up to
        // a certain range, then they get clamped
        Clamp( Value.AsInteger, -Constants::GPUTextureSize, (2*Constants::GPUTextureSize)-1 );
        GPU.PointedRegion->HotspotX = Value.AsInteger;
    }
    
    // -----------------------------------------------------------------------------
    
    void WriteGPURegionHotspotY( V32GPU& GPU, V32Word Value )
    {
        // out of texture values are valid
        Clamp( Value.AsInteger, -Constants::GPUTextureSize, (2*Constants::GPUTextureSize)-1 );
        GPU.PointedRegion->HotspotY = Value.AsInteger;
    }
}