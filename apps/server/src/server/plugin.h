//
// Created by matheus on 2/2/26.
//

#ifndef CHATPP_PLUGIN_H
#define CHATPP_PLUGIN_H
#include <functional>
#include <string>

struct MessagePattern {
public:
    std::string pattern;
    std::function<void(std::string)> pluginAction;
};

class Plugin {
    public:
    Plugin() {

    }

    std::vector<MessagePattern> messagePattern;
};

#endif //CHATPP_PLUGIN_H