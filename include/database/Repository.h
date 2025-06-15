#pragma once
#include <string>
#include <memory>
#include <vector>
#include <stdexcept>

template <typename T>
class Repository
{
public:
    virtual ~Repository() = default;
    virtual void save(std::shared_ptr<T> entity) = 0;
    virtual std::shared_ptr<T> findById(const std::string &id) = 0;
    virtual std::vector<std::shared_ptr<T>> findAll() = 0;
    virtual void remove(const std::string &id) = 0;
};