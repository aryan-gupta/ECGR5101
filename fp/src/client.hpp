#pragma once

#include <string>

/// sends an image to the server. The image is denoted by 
/// the file system path in \p path
void client_send_data(std::string path);