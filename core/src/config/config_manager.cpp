#include <16nar/core/config/config_manager.h>

#include <16nar/platform/logger/logger.h>

#include <charconv>
#include <cstdio>
#include <memory>
#include <fstream>
#include <iomanip>

#include <ini.h>

#define __NARENGINE_SET_CONFIG_PARAM( TYPE, CPP_TYPE, ... ) \
     if ( auto *param = get_param( section, name ) ) \
     { \
          if ( param->type != TYPE \
               || ( locked_ && !( param->flags & ConfigFlags::Dynamic ) ) ) \
          { \
               return false; \
          } \
          param->value.emplace< CPP_TYPE >( __VA_ARGS__ ); \
          return true; \
     } \
     return false;


#define __NARENGINE_GET_CONFIG_PARAM( TYPE, CPP_TYPE, RET_TYPE ) \
     if ( const auto *param = get_param( section, name ) ) \
     { \
          if ( param->type != TYPE ) \
          { \
               return RET_TYPE{}; \
          } \
          if ( const auto *val = std::get_if< CPP_TYPE >( &( param->value ) ) ) \
          { \
               return *val; \
          } \
     } \
     return RET_TYPE{};


namespace _16nar::config
{

ConfigManager::ConfigManager( strings::NameTable& names, std::pmr::memory_resource& resource ):
     config_( &resource ), names_{ names }, resource_{ resource }, locked_{}
{}


bool ConfigManager::add_parameter( std::string_view section, std::string_view name,
     ConfigType type, ConfigFlags flags )
{
     auto section_name = names_.get_static_name( section );
     if ( section_name.empty() )
     {
          section_name = names_.add_name( section );
          if ( section_name.empty() )
          {
               LOG_16NAR_ERROR( "Cannot add config section '%*.s' to name table",
                    static_cast< int >( section.size() ), section.data() );
               return false;
          }
     }
     auto param_name = names_.get_static_name( name );
     if ( param_name.empty() )
     {
          param_name = names_.add_name( name );
          if ( section_name.empty() )
          {
               LOG_16NAR_ERROR( "Cannot add config parameter '%*.s' to name table",
                    static_cast< int >( name.size() ), name.data() );
               return false;
          }
     }
     auto& config_section = config_[ section_name ];
     const auto result = config_section.try_emplace( param_name, ConfigParam{} );
     if ( !result.second )
     {
          LOG_16NAR_ERROR( "Cannot add config parameter '%.*s' in section '%.*s'",
               static_cast< int >( name.size() ), name.data(),
               static_cast< int >( section.size() ), section.data() );
          return false;
     }
     result.first->second.type = type;
     result.first->second.flags = flags;
     return true;
}


bool ConfigManager::load_from_file( const std::filesystem::path& path )
{
     if ( !locked_ )
     {
          LOG_16NAR_ERROR( "Attempt to load confuguration while it is not locked" );
          return false;
     }

     auto deleter = []( FILE *file ){ std::fclose( file ); };
     auto handle = std::fopen( path.string().c_str(), "r" );
     if ( !handle )
     {
          LOG_16NAR_ERROR( "Cannot open configuration file" );
          return false;
     }
     std::unique_ptr< FILE, decltype( deleter ) > guard{ handle, deleter };
     const int ret = ini_parse_file( handle, &ConfigManager::parse_callback, this );
     if ( ret < 0 )
     {
          LOG_16NAR_ERROR( "Cannot parse configuration INI file, error %d", ret );
     }
     if ( ret > 0 )
     {
          LOG_16NAR_ERROR( "Cannot parse line %d of configuration INI file", ret );
     }
     return ( ret == 0 );
}


bool ConfigManager::save_to_file( const std::filesystem::path& path ) const
{
     std::ofstream out{ path };

     for ( const auto section_iter : config_ )
     {
          bool printed_section = false;
          for ( const auto param_iter : section_iter.second )
          {
               if ( !( param_iter.second.flags & ConfigFlags::Dynamic )
                    || ( param_iter.second.flags & ConfigFlags::Hidden ) )
               {
                    continue;
               }
               if ( !printed_section )
               {
                    out << '[' << names_.get_name( section_iter.first ) << ']' << std::endl;
                    printed_section = true;
               }
               switch ( param_iter.second.type )
               {
                    case ConfigType::Uint:
                    {
                         if ( const auto *val = std::get_if< std::uint64_t >(
                              &(  param_iter.second.value ) ) )
                         {
                              out << names_.get_name( param_iter.first ) << '=' << *val << std::endl;
                         }
                         break;
                    }
                    case ConfigType::Int:
                    {
                         if ( const auto *val = std::get_if< std::int64_t >(
                              &(  param_iter.second.value ) ) )
                         {
                              out << names_.get_name( param_iter.first ) << '=' << *val << std::endl;
                         }
                         break;
                    }
                    case ConfigType::Float:
                    {
                         if ( const auto *val = std::get_if< double >(
                              &(  param_iter.second.value ) ) )
                         {
                              out << names_.get_name( param_iter.first ) << '=' << *val << std::endl;
                         }
                         break;
                    }
                    case ConfigType::Bool:
                    {
                         if ( const auto *val = std::get_if< bool >(
                              &(  param_iter.second.value ) ) )
                         {
                              out << names_.get_name( param_iter.first ) << '='
                                   << std::boolalpha << *val << std::endl;
                         }
                         break;
                    }
                    case ConfigType::String:
                    {
                         if ( const auto *val = std::get_if< std::pmr::string >(
                              &(  param_iter.second.value ) ) )
                         {
                              out << names_.get_name( param_iter.first ) << '=' << *val << std::endl;
                         }
                         break;
                    }
               }
          }
     }
     return true;
}


void ConfigManager::lock_config() noexcept
{
     locked_ = true;
}


std::uint64_t ConfigManager::get_uint( strings::StaticName section, strings::StaticName name ) const noexcept
{
     __NARENGINE_GET_CONFIG_PARAM( ConfigType::Uint, std::uint64_t, std::uint64_t );
}


std::int64_t ConfigManager::get_int( strings::StaticName section, strings::StaticName name ) const noexcept
{
     __NARENGINE_GET_CONFIG_PARAM( ConfigType::Int, std::int64_t, std::int64_t );
}


double ConfigManager::get_float( strings::StaticName section, strings::StaticName name ) const noexcept
{
     __NARENGINE_GET_CONFIG_PARAM( ConfigType::Float, double, double );
}


bool ConfigManager::get_bool( strings::StaticName section, strings::StaticName name ) const noexcept
{
     __NARENGINE_GET_CONFIG_PARAM( ConfigType::Bool, bool, bool );
}


std::string_view ConfigManager::get_string( strings::StaticName section, strings::StaticName name ) const noexcept
{
     __NARENGINE_GET_CONFIG_PARAM( ConfigType::String, std::pmr::string, std::string_view );
}


bool ConfigManager::set_uint( strings::StaticName section, strings::StaticName name, std::uint64_t value )
{
     __NARENGINE_SET_CONFIG_PARAM( ConfigType::Uint, std::uint64_t, value );
}


bool ConfigManager::set_int( strings::StaticName section, strings::StaticName name, std::int64_t value )
{
     __NARENGINE_SET_CONFIG_PARAM( ConfigType::Int, std::int64_t, value );
}


bool ConfigManager::set_float( strings::StaticName section, strings::StaticName name, double value )
{
     __NARENGINE_SET_CONFIG_PARAM( ConfigType::Float, double, value );
}


bool ConfigManager::set_bool( strings::StaticName section, strings::StaticName name, bool value )
{
     __NARENGINE_SET_CONFIG_PARAM( ConfigType::Bool, bool, value );
}


bool ConfigManager::set_string( strings::StaticName section, strings::StaticName name, std::string_view value )
{
     __NARENGINE_SET_CONFIG_PARAM( ConfigType::String, std::pmr::string,
          value.cbegin(), value.cend(), &resource_ );
}


int ConfigManager::parse_callback( void *user, const char *section, const char *name, const char *value )
{
     ConfigManager& config{ *reinterpret_cast< ConfigManager * >( user ) };
     const std::string_view section_str{ section };
     const std::string_view name_str{ name };
     const std::string_view value_str{ value };

     if ( section_str.empty() )
     {
          LOG_16NAR_ERROR( "Parameter '%s' must be within a section", name );
          return 0;
     }
     const auto section_sn = config.names_.get_static_name( section_str );
     if ( section_sn.empty() )
     {
          LOG_16NAR_ERROR( "Cannot find section name '%s' in a name table", section );
          return 0;
     }
     const auto name_sn = config.names_.get_static_name( name_str );
     if ( name_sn.empty() )
     {
          LOG_16NAR_ERROR( "Cannot find parameter '%s' in a name table", name );
          return 0;
     }

     const auto section_iter = config.config_.find( section_sn );
     if ( section_iter == config.config_.end() )
     {
          LOG_16NAR_ERROR( "Cannot find section '%s'", section );
          return 0;
     }
     const auto param_iter = section_iter->second.find( name_sn );
     if ( section_iter == config.config_.end() )
     {
          LOG_16NAR_ERROR( "Cannot find parameter '%s' in section '%s'", name, section );
          return 0;
     }

     if ( !( param_iter->second.flags & ConfigFlags::Dynamic ) )
     {
          LOG_16NAR_ERROR( "Static parameter '%s' in section '%s' must not be set from file", name, section );
          return 0;
     }

     const auto type = param_iter->second.type;
     switch ( type )
     {
          case ConfigType::Uint:
          {
               std::uint64_t value{};
               auto [ ptr, ec ] = std::from_chars( value_str.data(), value_str.data() + value_str.size(), value );
               if ( ec != std::errc{} )
               {
                    LOG_16NAR_ERROR( "Cannot parse Uint value of parameter '%s' in section '%s'", name, section );
                    return 0;
               }
               param_iter->second.value.emplace< std::uint64_t >( value );
               break;
          }
          case ConfigType::Int:
          {
               std::int64_t value{};
               auto [ ptr, ec ] = std::from_chars( value_str.data(), value_str.data() + value_str.size(), value );
               if ( ec != std::errc{} )
               {
                    LOG_16NAR_ERROR( "Cannot parse Int value of parameter '%s' in section '%s'", name, section );
                    return 0;
               }
               param_iter->second.value.emplace< std::int64_t >( value );
               break;
          }
          case ConfigType::Float:
          {
               double value{};
               auto [ ptr, ec ] = std::from_chars( value_str.data(), value_str.data() + value_str.size(), value );
               if ( ec != std::errc{} )
               {
                    LOG_16NAR_ERROR( "Cannot parse Float value of parameter '%s' in section '%s'", name, section );
                    return 0;
               }
               param_iter->second.value.emplace< double >( value );
               break;
          }
          case ConfigType::Bool:
          {
               bool value{};
               if ( value_str == "true" || value_str == "True" )
               {
                    value = true;
               }
               else if ( value_str == "false" || value_str == "False" )
               {
                    value = false;
               }
               else
               {
                    LOG_16NAR_ERROR( "Cannot parse Bool value of parameter '%s' in section '%s'", name, section );
                    return 0;
               }
               param_iter->second.value.emplace< bool >( value );
               break;
          }
          case ConfigType::String:
          {
               param_iter->second.value.emplace< std::pmr::string >(
                    value_str.cbegin(), value_str.cend(), &config.resource_ );
               break;
          }
          default:
          {
               LOG_16NAR_ERROR( "Unknown type of parameter '%s' in section '%s'", name, section );
               return 0;
          }
     }
     param_iter->second.flags = static_cast< ConfigFlags >( param_iter->second.flags & ~ConfigFlags::Hidden );
     return 1;
}


const ConfigManager::ConfigParam *ConfigManager::get_param(
     strings::StaticName section, strings::StaticName name ) const noexcept
{
     const auto section_iter = config_.find( section );
     if ( section_iter == config_.cend() )
     {
          return nullptr;
     }
     const auto param_iter = section_iter->second.find( name );
     if ( param_iter == section_iter->second.cend() )
     {
          return nullptr;
     }
     return &( param_iter->second );
}


ConfigManager::ConfigParam *ConfigManager::get_param(
     strings::StaticName section, strings::StaticName name ) noexcept
{
     const auto section_iter = config_.find( section );
     if ( section_iter == config_.end() )
     {
          return nullptr;
     }
     const auto param_iter = section_iter->second.find( name );
     if ( param_iter == section_iter->second.end() )
     {
          return nullptr;
     }
     return &( param_iter->second );
}

} // namespace _16nar::config
