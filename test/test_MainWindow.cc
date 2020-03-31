#include "MainWindow.h"

#include <clocale>
#include <exception>
#include <iostream>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"


int main (int argc, const char** argv)
{
  try
  {
    auto file_logger = spdlog::basic_logger_mt("basic_logger", "logs/basic-log.txt", true); // truncate
    spdlog::set_default_logger(file_logger);
    
    std::setlocale(LC_ALL, "");

    ncurses::MainWindow w;

    w.runForever ();
    
  }
  catch (const std::exception& e)
  {
    std::cout << "std::exception: " << e.what () << std::endl;
  }
  catch (...)
  {
    std::cout << "unknown exception" << std::endl;
  }
  
  return 0;
}
