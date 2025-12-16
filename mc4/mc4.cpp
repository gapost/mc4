#include "mc4.h"

#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <sstream>
#include <string>

#include <gpib/gpib_user.h>
#include <gpib/ib.h>

const char *mc4::gpib_err(int code)
{
    static const char *message[] = {"EDVR: System error",
                                    "ECIC: Function requires GPIB interface to be CIC",
                                    "ENOL: No Listeners on the GPIB",
                                    "EADR: GPIB interface not addressed correctly",
                                    "EARG: Invalid argument to function call",
                                    "ESAC: GPIB interface not System Controller as required",
                                    "EABO: I/O operation aborted (timeout)",
                                    "ENEB: Nonexistent GPIB interface",
                                    "EDMA: DMA error",
                                    "Unknown Error",
                                    "EOIP: Asynchronous I/O in progress",
                                    "ECAP: No capability for operation",
                                    "EFSO: File system error",
                                    "Unknown Error",
                                    "EBUS: GPIB bus error",
                                    "ESTB: Serial poll status byte queue overflow",
                                    "ESRQ: SRQ stuck in ON position",
                                    "Unknown Error",
                                    "Unknown Error",
                                    "Unknown Error",
                                    "ETAB: Table problem",
                                    "Unknown Error"};
    const int N_ERROR_CODES = 22;

    int error_code = (code < 0 || code >= N_ERROR_CODES) ? N_ERROR_CODES - 1 : code;
    return message[error_code];
}

int mc4::check_error()
{
    if (ibsta & ERR) {
        error_msg_ = gpib_err(iberr);
        return -1;
    } else {
        error_msg_.clear();
        return 0;
    }
}

mc4::mc4()
    : buff_(128, 0)
    , addr_(-1)
    , dev_(-1)
{}

int mc4::init(int addr)
{
    addr_ = addr;
    int eos = REOS | '\n'; /*finish character reading with LF */
    int eoi = 1;
    /* obtain device descriptor    */
    dev_ = ibdev(0, addr_, 0, 0, eoi, eos);
    if (dev_ < 0) {
        addr_ = -1;
        dev_ = -1;
        return check_error();
    }
    return 0;
}

int mc4::write(const std::string &cmd)
{
    std::string buff(cmd);
    buff += '\r';
    ibwrt(dev_, buff.data(), buff.size());
    return check_error();
}

int mc4::read(std::string &resp)
{
    std::vector<char> buff(71, 0);
    ibrd(dev_, buff.data(), 70);
    int ret = check_error();
    if (ret==0)
        resp = buff.data();
    return ret;
}

int mc4::close()
{
    if (dev_ < 0)
        return 0;

    ibonl(dev_, 0);
    addr_ = -1;
    dev_ = -1;
    return check_error();
}

int mc4::set_position(char axis, int pos)
{
    std::string cmd("P");
    cmd += static_cast<char>(std::toupper(static_cast<unsigned char>(axis)));
    cmd += std::to_string(pos);
    return write(cmd);
}

int mc4::get_position(char axis, int &pos)
{
    std::string cmd("D"), resp;
    cmd += static_cast<char>(std::toupper(static_cast<unsigned char>(axis)));
    int ret = write(cmd);
    if (ret != 0)
        return ret;
    ret = read(resp);
    if (ret != 0)
        return ret;
    write(cmd); // to silence mc4
    std::istringstream is(resp);
    char a;
    is.get(a);
    is.get(a);
    is >> pos;
    return is.fail() ? -1 : 0;
}

int mc4::reset_position(char axis)
{
    std::string cmd("D"), resp;
    cmd += static_cast<char>(std::toupper(static_cast<unsigned char>(axis)));
    int ret = write(cmd);
}

int mc4::get_status(char axis, char &status) {}
