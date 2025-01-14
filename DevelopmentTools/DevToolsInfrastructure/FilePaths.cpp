// *****************************************************************************
    // include project headers
    #include "FilePaths.hpp"
    
    // include C/C++ headers
    #include <iostream>         // [ C++ STL ] I/O Streams
    #include <fstream>          // [ C++ STL ] File streams
    #include <algorithm>        // [ C++ STL ] Algorithms
    #include <sys/stat.h>       // [ ANSI C ] File status
    
    // declare used namespaces
    using namespace std;
// *****************************************************************************


// =============================================================================
//      PATH SEPARATOR CHARACTER
// =============================================================================


// this is dependent on the host operating system
#if defined(__WIN32__) || defined(_WIN32) || defined(_WIN64)
    char PathSeparator = '\\';
#else
    char PathSeparator = '/';
#endif

// -----------------------------------------------------------------------------

// auxiliary function to unify forward and back slashes
string NormalizePathSeparators( const string& FilePath )
{
    string NormalizedPath = FilePath;
    
    for( auto& c: NormalizedPath )
    {
        #if defined(__WIN32__) || defined(_WIN32) || defined(_WIN64)
            if( c == '/' ) c = '\\';
        #else
            if( c == '\\' ) c = '/';
        #endif
    }
    
    return NormalizedPath;
}


// =============================================================================
//      STRING MANIPULATION FUNCTIONS
// =============================================================================


string GetFileExtension( const string& FilePath )
{
    size_t DotPosition = FilePath.rfind( '.' );
    
    if( DotPosition == string::npos )
      return "";
    
    return FilePath.substr( DotPosition+1 );
}

// -----------------------------------------------------------------------------

// returns extension without the dot
string GetFileWithoutExtension( const string& FilePath )
{
    size_t DotPosition = FilePath.rfind( '.' );
    
    if( DotPosition == string::npos )
      return "";
    
    return FilePath.substr( 0, DotPosition );
}

// -----------------------------------------------------------------------------

string ReplaceFileExtension( const string& FilePath, const string& NewExtension )
{
    string OriginalExtension = GetFileExtension( FilePath );
    string PathWithoutExtension = FilePath.substr( 0, FilePath.length() - OriginalExtension.length() );
    return PathWithoutExtension + NewExtension;
}

// -----------------------------------------------------------------------------

string GetPathDirectory( const string& FilePath )
{
    string NormalizedPath = NormalizePathSeparators( FilePath );
    size_t SlashPosition = NormalizedPath.rfind( PathSeparator );
    
    // careful, if the path is empty (i.e. current folder)
    // we need to return a dot or else paths will be wrong
    if( SlashPosition == string::npos )
      return string(".") + PathSeparator;
    
    return NormalizedPath.substr( 0, SlashPosition+1 );
}

// -----------------------------------------------------------------------------

string GetPathFileName( const string& FilePath )
{
    string NormalizedPath = NormalizePathSeparators( FilePath );
    size_t SlashPosition = NormalizedPath.rfind( PathSeparator );
    
    if( SlashPosition == string::npos )
      return FilePath;
    
    if( NormalizedPath.size() < (SlashPosition+2) )
      return "";
    
    return NormalizedPath.substr( SlashPosition+1, NormalizedPath.size()-1 );
}


// =============================================================================
//      CHECKING VALIDITY OF PATHS
// =============================================================================


bool IsFileNameValid( const string& FileName )
{
    if( FileName.find('\"') != string::npos )  return false;
    if( FileName.find('\\') != string::npos )  return false;
    if( FileName.find('/')  != string::npos )  return false;
    if( FileName.find('|')  != string::npos )  return false;
    if( FileName.find('<')  != string::npos )  return false;
    if( FileName.find('>')  != string::npos )  return false;
    if( FileName.find(':')  != string::npos )  return false;
    if( FileName.find('?')  != string::npos )  return false;
    if( FileName.find('*')  != string::npos )  return false;
    
    return true;
}

// -----------------------------------------------------------------------------

bool FileExists( const string& FilePath )
{
    struct stat Info;

    if( stat( FilePath.c_str(), &Info ) != 0 )
      return false;
    
    return !(Info.st_mode & S_IFDIR);
}

// -----------------------------------------------------------------------------

bool DirectoryExists( const string& Path )
{
    struct stat Info;

    if( stat( Path.c_str(), &Info ) != 0 )
      return false;
    
    return (Info.st_mode & S_IFDIR);
}


// =============================================================================
//      CREATING DIRECTORIES
// =============================================================================


// returns true on success
bool CreateDirectory( const std::string DirectoryPath )
{
    #if defined(__WIN32__) || defined(_WIN32) || defined(_WIN64)
      int Status = mkdir( DirectoryPath.c_str() );
    #else
      int Status = mkdir( DirectoryPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
    #endif
    
    return (Status >= 0);
}
