#ifndef ART_HPP
#define ART_HPP

#include <iostream>
#include <string>

class ArtMode {
private:
    bool enabled = false;

public:
    void set_enabled(bool value) {
        enabled = value;
        if (enabled) {
            show_welcome();
        }
    }

    bool is_enabled() const {
        return enabled;
    }
    
    void display(const std::string& msg) const {
        if (enabled) {
            std::cout << " " << std::endl;
            std::cout << "|| " << msg << "||" << std::endl;
        } else {
            std::cout << msg << "\n";
        }
    }

    void show_welcome() const {
        std::cout << "   .--.                                 " << std::endl;
        std::cout << " .-(    ).             _.__            .--.      " << std::endl;
        std::cout << "(___.__)__)       .--./    `        .-(    ).       " << std::endl;
        std::cout << "                /              |  (___.__)__)   " << std::endl;
        std::cout << "  _    _      _             \\ .-- . /           " << std::endl;
        std::cout << " | |  | |    | |          __/       \\__       " << std::endl;
        std::cout << " | |  | | ___| | ___ ___  _ __ ___   ___ " << std::endl;
        std::cout << " | |/\\| |/ _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\" << std::endl;
        std::cout << " \\  /\\  /  __/ | (_| (_) | | | | | |  __/" << std::endl;
        std::cout << "  \\/  \\/ \\___|_|\\___\\___/|_| |_| |_|\\___|" << std::endl;
        std::cout << "____________[ ART MODE IS ON ]_____________" << std::endl;
        std::cout << "        .--.            .--.            " << std::endl;
        std::cout << "     .-(    ).      .-(    ).          " << std::endl;
        std::cout << "    (___.__)__)    (___.__)__)         " << std::endl;
    }

    void show_bye() const {
        std::cout << " .----------------.  .----------------.  .----------------.  .----------------. " << std::endl;
        std::cout << "| .--------------. || .--------------. || .--------------. || .--------------. |" << std::endl;
        std::cout << "| |   ______     | || |  ____  ____  | || |  _________   | || |              | |" << std::endl;
        std::cout << "| |  |_   _ \\    | || | |_  _||_  _| | || | |_   ___  |  | || |      _       | |" << std::endl;
        std::cout << "| |    | |_) |   | || |   \\ \\  / /   | || |   | |_  \\_|  | || |     | |      | |" << std::endl;
        std::cout << "| |    |  __'.   | || |    \\ \\/ /    | || |   |  _|  _   | || |     | |      | |" << std::endl;
        std::cout << "| |   _| |__) |  | || |    _|  |_    | || |  _| |___/ |  | || |     | |      | |" << std::endl;
        std::cout << "| |  |_______/   | || |   |______|   | || | |_________|  | || |     |_|      | |" << std::endl;
        std::cout << "| |              | || |              | || |              | || |     (_)      | |" << std::endl;
        std::cout << "| '--------------' || '--------------' || '--------------' || '--------------' |" << std::endl;
        std::cout << " '----------------'  '----------------'  '----------------'  '----------------' " << std::endl;
        std::cout << "                               _.-=-._     .-, " << std::endl;
        std::cout << "                             .' 0 )     \"-,.' / " << std::endl;
        std::cout << "♥ ︎Was a joy working with you ︎(    )      _.  < " << std::endl;
        std::cout << "                              `=._)___.=\'  `._\\" << std::endl;
    }
    
    void show_version_tree_bubbles(tree_node* root, const std::string& prefix = "", bool is_last = true) const {
        if (!root) return;
    
        std::string branch = is_last ? "└─ " : "├─ ";
        std::string bubble_prefix = prefix + (is_last ? "    " : "│   ");
    
        std::cout << prefix << branch << "   /`````\\ " << std::endl;
        std::cout << bubble_prefix << " |  V" << root->version_id << "   |" << std::endl;
        std::cout << bubble_prefix << "  \\_____/ " << std::endl;
    
        if (!root->message.empty())
            std::cout << bubble_prefix << "\"" << root->message << "\"" << std::endl;
    
        if (!root->children.empty()) {
            std::cout << bubble_prefix << "   |" << std::endl;
        }
    
        for (size_t i = 0; i < root->children.size(); ++i) {
            show_version_tree_bubbles(root->children[i], prefix + (is_last ? "    " : "│   "), i == root->children.size() - 1);
        }
    }




};

#endif // ART_HPP
