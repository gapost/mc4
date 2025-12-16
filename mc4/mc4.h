#ifndef MC4_H
#define MC4_H

#include <string>
#include <vector>

class mc4
{
    std::string error_msg_;
    int addr_;
    int dev_;

    static const char *gpib_err(int code);
    int check_error();

public:
    mc4();
    int init(int addr);
    int write(const std::string &cmd);
    int read(std::string &resp);
    const std::string &error_msg() const { return error_msg_; }
    int close();

    int set_position(char axis, int pos);
    int get_position(char axis, int &pos);
    int reset_position(char axis);
    int get_status(char moving[4], char limitup[4], char limitdwn[4], int pos[4]);
};

#endif // MC4_H
