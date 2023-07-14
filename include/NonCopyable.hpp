#ifndef __NON_COPYABLE_HPP
#define __NON_COPYABLE_HPP

class NonCopyAble {
protected:
    NonCopyAble() = default;
    ~NonCopyAble() = default;

private:
    NonCopyAble(const NonCopyAble&) = delete;
    NonCopyAble& operator=(const NonCopyAble&) = delete;
};

#endif //__NON_COPYABLE_HPP
