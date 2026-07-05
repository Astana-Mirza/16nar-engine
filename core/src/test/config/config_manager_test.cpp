#include <catch2/catch_test_macros.hpp>

#include <16nar/platform/strings/name_table.h>
#include <16nar/core/config/config_manager.h>

TEST_CASE( "Parameters registration", "[config_manager]" )
{
     _16nar::strings::NameTable name_table{ *std::pmr::get_default_resource() };
     _16nar::config::ConfigManager config{ name_table, *std::pmr::get_default_resource() };

     CHECK( config.add_parameter( "static", "test_static_uint",
          _16nar::config::ConfigManager::ConfigType::Uint,
          _16nar::config::ConfigManager::ConfigFlags::None ) );
     CHECK( config.add_parameter( "static", "test_static_int",
          _16nar::config::ConfigManager::ConfigType::Int,
          _16nar::config::ConfigManager::ConfigFlags::None ) );
     CHECK( config.add_parameter( "static", "test_static_float",
          _16nar::config::ConfigManager::ConfigType::Float,
          _16nar::config::ConfigManager::ConfigFlags::None ) );
     CHECK( config.add_parameter( "static", "test_static_bool",
          _16nar::config::ConfigManager::ConfigType::Bool,
          _16nar::config::ConfigManager::ConfigFlags::None ) );
     CHECK( config.add_parameter( "static", "test_static_string",
          _16nar::config::ConfigManager::ConfigType::String,
          _16nar::config::ConfigManager::ConfigFlags::None ) );
     CHECK( config.add_parameter( "dynamic", "test_dynamic_uint",
          _16nar::config::ConfigManager::ConfigType::Uint,
          _16nar::config::ConfigManager::ConfigFlags::Dynamic ) );
     CHECK( config.add_parameter( "dynamic", "test_dynamic_int",
          _16nar::config::ConfigManager::ConfigType::Int,
          _16nar::config::ConfigManager::ConfigFlags::Dynamic ) );
     CHECK( config.add_parameter( "dynamic", "test_dynamic_float",
          _16nar::config::ConfigManager::ConfigType::Float,
          _16nar::config::ConfigManager::ConfigFlags::Dynamic ) );
     CHECK( config.add_parameter( "dynamic", "test_dynamic_bool",
          _16nar::config::ConfigManager::ConfigType::Bool,
          _16nar::config::ConfigManager::ConfigFlags::Dynamic ) );
     CHECK( config.add_parameter( "dynamic", "test_dynamic_string",
          _16nar::config::ConfigManager::ConfigType::String,
          _16nar::config::ConfigManager::ConfigFlags::Dynamic ) );

     // no duplicates allowed
     CHECK( !config.add_parameter( "dynamic", "test_dynamic_string",
          _16nar::config::ConfigManager::ConfigType::String,
          _16nar::config::ConfigManager::ConfigFlags::Dynamic ) );

     // duplicate names in different sections are allowed
     CHECK( config.add_parameter( "another_section", "test_static_string",
          _16nar::config::ConfigManager::ConfigType::String,
          _16nar::config::ConfigManager::ConfigFlags::None ) );

     const _16nar::strings::StaticName static_section{ "static" };
     const _16nar::strings::StaticName dynamic_section{ "dynamic" };

     // check default values
     CHECK( 0ULL == config.get_uint( static_section,
          _16nar::strings::StaticName{ "test_static_uint" } ) );
     CHECK( 0 == config.get_int( static_section,
          _16nar::strings::StaticName{ "test_static_int" } ) );
     CHECK( 0.0 == config.get_float( static_section,
          _16nar::strings::StaticName{ "test_static_float" } ) );
     CHECK( std::string_view{} == config.get_string( static_section,
          _16nar::strings::StaticName{ "test_static_string" } ) );
     CHECK( 0ULL == config.get_uint( static_section,
          _16nar::strings::StaticName{ "not_exists" } ) );

     CHECK( config.set_uint( static_section,
          _16nar::strings::StaticName{ "test_static_uint" }, 123ULL ) );
     CHECK( config.set_int( static_section,
          _16nar::strings::StaticName{ "test_static_int" }, -456 ) );
     CHECK( config.set_float( static_section,
          _16nar::strings::StaticName{ "test_static_float" }, -21.5 ) );
     CHECK( config.set_bool( static_section,
          _16nar::strings::StaticName{ "test_static_bool" }, true ) );
     CHECK( config.set_string( static_section,
          _16nar::strings::StaticName{ "test_static_string" }, "moe!!!" ) );
     CHECK( config.set_uint( dynamic_section,
          _16nar::strings::StaticName{ "test_dynamic_uint" }, 103ULL ) );
     CHECK( config.set_int( dynamic_section,
          _16nar::strings::StaticName{ "test_dynamic_int" }, 567 ) );
     CHECK( config.set_float( dynamic_section,
          _16nar::strings::StaticName{ "test_dynamic_float" }, -12.5 ) );
     CHECK( config.set_bool( dynamic_section,
          _16nar::strings::StaticName{ "test_dynamic_bool" }, true ) );
     CHECK( config.set_string( dynamic_section,
          _16nar::strings::StaticName{ "test_dynamic_string" }, "dynamic_moe!!!" ) );

     config.lock_config();

     // change of static parameters is not allowed after lock
     CHECK( !config.set_uint( static_section,
          _16nar::strings::StaticName{ "test_static_uint" }, 1123ULL ) );
     CHECK( !config.set_int( static_section,
          _16nar::strings::StaticName{ "test_static_int" }, -4656 ) );
     CHECK( !config.set_float( static_section,
          _16nar::strings::StaticName{ "test_static_float" }, -261.5 ) );
     CHECK( !config.set_bool( static_section,
          _16nar::strings::StaticName{ "test_static_bool" }, false ) );
     CHECK( !config.set_string( static_section,
          _16nar::strings::StaticName{ "test_static_string" }, "!!!moe!!!" ) );

     CHECK( config.get_uint( static_section,
          _16nar::strings::StaticName{ "test_static_uint" } ) == 123ULL );
     CHECK( config.get_int( static_section,
          _16nar::strings::StaticName{ "test_static_int" } ) == -456 );
     CHECK( config.get_float( static_section,
          _16nar::strings::StaticName{ "test_static_float" } ) == -21.5 );
     CHECK( config.get_bool( static_section,
          _16nar::strings::StaticName{ "test_static_bool" } ) == true );
     CHECK( config.get_string( static_section,
          _16nar::strings::StaticName{ "test_static_string" } ) == "moe!!!" );
     CHECK( config.get_uint( dynamic_section,
          _16nar::strings::StaticName{ "test_dynamic_uint" } ) == 103ULL );
     CHECK( config.get_int( dynamic_section,
          _16nar::strings::StaticName{ "test_dynamic_int" } ) == 567 );
     CHECK( config.get_float( dynamic_section,
          _16nar::strings::StaticName{ "test_dynamic_float" } ) == -12.5 );
     CHECK( config.get_bool( dynamic_section,
          _16nar::strings::StaticName{ "test_dynamic_bool" } ) == true );
     CHECK( config.get_string( dynamic_section,
          _16nar::strings::StaticName{ "test_dynamic_string" } ) == "dynamic_moe!!!" );

     // change of dynamic parameters is allowed after lock
     CHECK( config.set_uint( dynamic_section,
          _16nar::strings::StaticName{ "test_dynamic_uint" }, 1083ULL ) );
     CHECK( config.set_int( dynamic_section,
          _16nar::strings::StaticName{ "test_dynamic_int" }, 5867 ) );
     CHECK( config.set_float( dynamic_section,
          _16nar::strings::StaticName{ "test_dynamic_float" }, -182.5 ) );
     CHECK( config.set_bool( dynamic_section,
          _16nar::strings::StaticName{ "test_dynamic_bool" }, false ) );
     CHECK( config.set_string( dynamic_section,
          _16nar::strings::StaticName{ "test_dynamic_string" }, "!!!dynamic_moe!!!" ) );

     CHECK( config.get_uint( dynamic_section,
          _16nar::strings::StaticName{ "test_dynamic_uint" } ) == 1083ULL );
     CHECK( config.get_int( dynamic_section,
          _16nar::strings::StaticName{ "test_dynamic_int" } ) == 5867 );
     CHECK( config.get_float( dynamic_section,
          _16nar::strings::StaticName{ "test_dynamic_float" } ) == -182.5 );
     CHECK( config.get_bool( dynamic_section,
          _16nar::strings::StaticName{ "test_dynamic_bool" } ) == false );
     CHECK( config.get_string( dynamic_section,
          _16nar::strings::StaticName{ "test_dynamic_string" } ) == "!!!dynamic_moe!!!" );
}


TEST_CASE( "Config file reading and writing", "[config_manager]" )
{
     _16nar::strings::NameTable name_table{ *std::pmr::get_default_resource() };
     _16nar::config::ConfigManager config{ name_table, *std::pmr::get_default_resource() };

     CHECK( config.add_parameter( "static", "test_static_uint",
          _16nar::config::ConfigManager::ConfigType::Uint,
          _16nar::config::ConfigManager::ConfigFlags::None ) );
     CHECK( config.add_parameter( "dynamic", "test_static_uint",
          _16nar::config::ConfigManager::ConfigType::Uint,
          _16nar::config::ConfigManager::ConfigFlags::None ) );

     CHECK( config.add_parameter( "dynamic", "test_uint",
          _16nar::config::ConfigManager::ConfigType::Uint,
          _16nar::config::ConfigManager::ConfigFlags::Dynamic ) );
     CHECK( config.add_parameter( "dynamic", "test_int",
          _16nar::config::ConfigManager::ConfigType::Int,
          _16nar::config::ConfigManager::ConfigFlags::Dynamic ) );
     CHECK( config.add_parameter( "dynamic", "test_float",
          _16nar::config::ConfigManager::ConfigType::Float,
          _16nar::config::ConfigManager::ConfigFlags::Dynamic ) );
     CHECK( config.add_parameter( "dynamic", "test_bool",
          _16nar::config::ConfigManager::ConfigType::Bool,
          _16nar::config::ConfigManager::ConfigFlags::Dynamic ) );
     CHECK( config.add_parameter( "dynamic", "test_string",
          _16nar::config::ConfigManager::ConfigType::String,
          _16nar::config::ConfigManager::ConfigFlags::Dynamic ) );

     CHECK( !config.load_from_file( "data/configs/good_config1.ini" ) );
     config.lock_config();
     CHECK( config.load_from_file( "data/configs/good_config1.ini" ) );

     const _16nar::strings::StaticName dynamic_section{ "dynamic" };
     CHECK( config.get_uint( dynamic_section,
          _16nar::strings::StaticName{ "test_uint" } ) == 145ULL );
     CHECK( config.get_int( dynamic_section,
          _16nar::strings::StaticName{ "test_int" } ) == -25 );
     CHECK( config.get_float( dynamic_section,
          _16nar::strings::StaticName{ "test_float" } ) == 125.5 );
     CHECK( config.get_bool( dynamic_section,
          _16nar::strings::StaticName{ "test_bool" } ) == true );
     CHECK( config.get_string( dynamic_section,
          _16nar::strings::StaticName{ "test_string" } ) == "my test string" );

     CHECK( config.save_to_file( "data/configs/out_good_config1.ini" ) );

     CHECK( !config.load_from_file( "data/configs/bad_config1.ini" ) );
     CHECK( config.get_uint( dynamic_section,
          _16nar::strings::StaticName{ "test_uint" } ) == 145ULL );
     CHECK( config.get_int( dynamic_section,
          _16nar::strings::StaticName{ "test_int" } ) == -25 );

     CHECK( !config.load_from_file( "data/configs/bad_config2.ini" ) );
     CHECK( config.get_uint( dynamic_section,
          _16nar::strings::StaticName{ "test_uint" } ) == 145ULL );
     CHECK( config.get_int( dynamic_section,
          _16nar::strings::StaticName{ "test_int" } ) == -25 );

     CHECK( !config.load_from_file( "data/configs/bad_config3.ini" ) );
     CHECK( config.get_uint( dynamic_section,
          _16nar::strings::StaticName{ "test_uint" } ) == 145ULL );
     CHECK( config.get_int( dynamic_section,
          _16nar::strings::StaticName{ "test_int" } ) == -25 );

     CHECK( !config.load_from_file( "data/configs/bad_config4.ini" ) );
     CHECK( config.get_uint( dynamic_section,
          _16nar::strings::StaticName{ "test_uint" } ) == 145ULL );
     CHECK( config.get_int( dynamic_section,
          _16nar::strings::StaticName{ "test_int" } ) == -25 );

     CHECK( !config.load_from_file( "data/configs/good_config2.ini" ) );
     CHECK( config.get_uint( dynamic_section,
          _16nar::strings::StaticName{ "test_uint" } ) == 14425ULL );
     CHECK( config.get_string( dynamic_section,
          _16nar::strings::StaticName{ "test_string" } ) == "\"test \\; config\"" );

     CHECK( config.save_to_file( "data/configs/out_good_config2.ini" ) );
}
