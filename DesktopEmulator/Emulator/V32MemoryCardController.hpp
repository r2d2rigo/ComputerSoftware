// *****************************************************************************
    // start include guard
    #ifndef V32MEMORYCARDCONTROLLER_HPP
    #define V32MEMORYCARDCONTROLLER_HPP
    
    // include project headers
    #include "V32Buses.hpp"
    #include "V32Memory.hpp"
    
    // include C/C++ headers
    #include <string>           // [ C++ STL ] Strings
// *****************************************************************************


namespace V32
{
    // =============================================================================
    //      STORAGE CONTROLLER DEFINITIONS
    // =============================================================================
    
    
    // LOCAL port numbers!
    // (to be added the initial address)
    enum class MEM_LocalPorts: int32_t
    {
        Connected = 0
    };
    
    // used as limit of local port numbers
    const int32_t MEM_LastPort = (int32_t)MEM_LocalPorts::Connected;
    
    
    // =============================================================================
    //      MEMORY CARD CONTROLLER CLASS
    // =============================================================================
    
    
    class V32MemoryCardController: public VirconControlInterface, public V32RAM
    {
        public:
            
            // file save control
            std::string CardFileName;
            std::string CardSavePath;
            bool PendingSave;
            
        public:
            
            // instance handling
            V32MemoryCardController();
            
            // connection to control bus
            virtual bool ReadPort( int32_t LocalPort, V32Word& Result );
            virtual bool WritePort( int32_t LocalPort, V32Word Value );
            
            // connection to memory bus (overriden)
            virtual bool WriteAddress( int32_t LocalAddress, V32Word Value );
            
            // memory contents (overriden)
            virtual void LoadContents( const std::string& FilePath );
            virtual void SaveContents( const std::string& FilePath );
            
            // additional method for creating a new file
            void CreateNewFile( const std::string& FilePath );
            
            // general operation
            void ChangeFrame();
    };
}


// *****************************************************************************
    // end include guard
    #endif
// *****************************************************************************