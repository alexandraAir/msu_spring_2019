#pragma once

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
    static constexpr char Separator = ' ';
    std::ostream& out_;
 
    Error write(const bool value) {
        if (value)
            out_<<"true"<<Separator;
        else
            out_<<"false"<<Separator;

        return Error::NoError;
    }
    
    Error write(const uint64_t value) {
        out_<<value<<Separator;
        return Error::NoError;
    }
    
    
    template <class T, class... Args>
    Error process(T &&value, Args&&... args) {
        if (write(std::forward<T>(value)) == Error::NoError)
            if (process(std::forward<Args>(args)...) == Error::NoError)
                return Error::NoError;

        return Error::CorruptedArchive;
    }
    
    Error process() { return Error::NoError; }
    
public:
    explicit Serializer(std::ostream& out) : out_(out) {}
    
    template <class T>
    Error save(T& object) { return object.serialize(*this); }
    
    template <class... Args>
    Error operator()(Args&&... args) {
        return process(std::forward<Args>(args)...);
    }
    
};

class Deserializer
{
    std::istream& in_;
    
    Error read(bool& value) {
        std::string text;
        in_ >> text;
        
        if (text == "true")
            value = true;
        else if (text == "false")
            value = false;
        else
            return Error::CorruptedArchive;
        
        return Error::NoError;
    }
    
    Error read(uint64_t& value) {
        std::string str;
        in_>>str;
        if(str[0] == '-')
            return Error::CorruptedArchive;
        try {
            value = std::stoull(str);
        } catch (std::invalid_argument &arg) {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
            
    }
    
    
    template <class T, class... Args>
    Error process(T &&value, Args&&... args) {
        if (read(std::forward<T>(value)) == Error::NoError)
            if (process(std::forward<Args>(args)...) == Error::NoError)
                return Error::NoError;
        
        return Error::CorruptedArchive;
    }
    Error process() { return Error::NoError; }

    
public:
    
    explicit Deserializer(std::istream& in) : in_(in) {}
    
    template <class T>
    Error load(T& object) { return object.serialize(*this); }
    
    template <class... Args>
    Error operator()(Args&&... args) {
        return process(std::forward<Args>(args)...);
    }
    
};