#ifndef OBSERVER_H
#define OBSERVER_H

template<typename T>
class Observer {
public:
  virtual void Update(std::shared_ptr<T> data) = 0;
};

#endif /* OBSERVER_H */
