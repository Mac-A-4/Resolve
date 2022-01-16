#include <Resolve/Resolve.h>
#include <GetProcAddressEx.h>
#include <sstream>
#include <cctype>

static void ReadSpace(std::istream& _Input) {

    while (std::isspace(_Input.peek())) {

        _Input.get();

    }

}

static bool ReadReference(HANDLE _Process, std::istream& _Input, uintptr_t& _Return) {

    _Input.get();

    std::string mod_buf;

    for (char value = _Input.get(); value != '>'; value = _Input.get()) {

        mod_buf.push_back(value);

    }

    ReadSpace(_Input);

    if (_Input.get() != ':')
        return false;

    ReadSpace(_Input);

    std::string name_buf;

    for (char value = _Input.peek(); std::isalnum(value) || value == '_'; _Input.get(), value = _Input.peek()) {

        name_buf.push_back(value);

    }

    _Return = reinterpret_cast<uintptr_t>(GetProcAddressEx(_Process, mod_buf.c_str(), name_buf.c_str()));

    return _Return != 0;

}

#ifdef _WIN64

#define stoptr std::stoull

#else

#define stoptr std::stoul

#endif

static bool ReadDecOffset(std::istream& _Input, uintptr_t& _Return) {

    std::string buf;

    for (char value = _Input.peek(); std::isdigit(value); _Input.get(), value = _Input.peek()) {

        buf.push_back(value);

    }

    size_t size;

    _Return = stoptr(buf, &size, 10);

    return size == buf.length();

}

static bool ReadHexOffset(std::istream& _Input, uintptr_t& _Return) {

    std::string buf;

    for (char value = _Input.peek(); std::isxdigit(value); _Input.get(), value = _Input.peek()) {

        buf.push_back(value);

    }

    size_t size;

    _Return = stoptr(buf, &size, 16);

    return size == buf.length();

}

static bool ReadToken(HANDLE _Process, std::istream& _Input, uintptr_t& _Return) {

    char first = _Input.peek();

    if (first == '<') {

        return ReadReference(_Process, _Input, _Return);

    }
    else if (first == '0') {

        return ReadHexOffset(_Input, _Return);

    }
    else if (std::isdigit(first)) {

        return ReadDecOffset(_Input, _Return);

    }
    else {

        return false;

    }

}

bool Resolve(HANDLE _Process, const std::string& _Expression, uintptr_t& _Return) {

    std::istringstream input(_Expression);

    ReadSpace(input);

    if (!ReadToken(_Process, input, _Return))
        return false;
    
    while (true) {

        ReadSpace(input);

        switch (input.get()) {

        case '+': {

            uintptr_t offset;

            ReadSpace(input);

            if (!ReadToken(_Process, input, offset))
                return false;

            _Return += offset;

            break;

        }

        case '-': {

            uintptr_t offset;

            ReadSpace(input);

            if (!ReadToken(_Process, input, offset))
                return false;

            _Return -= offset;

            break;

        }

        case EOF:

            return true;

        }

    }

}