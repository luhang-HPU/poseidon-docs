//
// Created by lxs on 24-2-21.
//

#ifndef POSEIDON_EXCEPTION_H
#define POSEIDON_EXCEPTION_H
#include <iostream>
#include <string>
#include <exception>
using namespace std;
namespace poseidon{



    class poseidon_error : public std::runtime_error{
    public:
        poseidon_error(const string &filename, uint32_t line_num, const string &what)
         : std::runtime_error(what),filename_(filename),line_num_(line_num)
        {
            comb_message_ = filename_ + ":" + std::to_string(line_num_) + " " + what;
        }

        const char* what() const noexcept
        {
            return comb_message_.c_str();
        }


        const uint32_t GetLineNum(){
            return line_num_;
        }

        const string &GetFileNum(){
            return filename_;
        }

    private:
        string filename_;
        uint32_t line_num_;
        string comb_message_;
    };


    class metadata_error : public poseidon_error{
    public:
        metadata_error(const string &filename, uint32_t line_num, const string &what)
         :poseidon_error(filename,line_num,what)
        {}
    };

    class config_error : public poseidon_error{
    public:
        config_error(const string &filename, uint32_t line_num, const string &what)
          :poseidon_error(filename,line_num,what)
        {}
    };

    class invalid_error : public poseidon_error{
    public:
        invalid_error(const string &filename, uint32_t line_num, const string &what)
          :poseidon_error(filename,line_num,what)
        {}
    };

#define POSEIDON_THROW(exc,expr) throw exc(__FILE__,__LINE__,(expr))
}



#endif //POSEIDON_EXCEPTION_H
