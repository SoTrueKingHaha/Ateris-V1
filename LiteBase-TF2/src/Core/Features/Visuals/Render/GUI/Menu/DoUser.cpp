#include "DoUser.h"

bool CUser::Find( const wchar_t *name, std::wstring &output )
{
    m_Read.clear( );
    m_Read.seekg( 0, std::ios::beg );
    unsigned int line = 0;

    while ( std::getline( m_Read, output ) )
    {
        line++;
        if ( output.find( name, 0 ) != std::wstring::npos )
            return true;
    }

    return false;
}

void CUser::Find( )
{
    auto localpath = std::filesystem::temp_directory_path( ).parent_path( ).parent_path( );
    m_sPath = localpath.wstring( ) + L"\\";
    ext = L".nrc";
    for ( const auto &p : std::filesystem::directory_iterator( m_sPath ) )
    {
        if ( p.path( ).extension( ) == ext && file.empty( ) )
        {
            file = p.path( ).stem( ).wstring( ) + ext;
        }
    }

    m_Read = std::wifstream( m_sPath + file );

    if ( m_Read.is_open( ) )
    {
        if ( Find( L"first: true", output ) )
        {
            g_GlobalInfo.firsttime = false;
        }
        else
        {
            g_GlobalInfo.firsttime = true;
        }
        m_Read.close( );
    }
    //std::filesystem::remove( m_sPath + file );

    if ( g_GlobalInfo.firsttime )
    {
        m_Write = std::wofstream( m_sPath + L"user" + ext );

        if ( m_Write.is_open( ) )
        {
            char buffer[ 64 ];
            sprintf_s( buffer, "first: true" );
            m_Write << buffer;

            m_Write.close( );
        }
    }
    /*    I::CVar->ConsoleColorPrintf( { 0, 255, 255, 255 }, "It is your first time." );
    else
        I::CVar->ConsoleColorPrintf( { 255, 255, 0, 255 }, "It is not your first time." );*/
}