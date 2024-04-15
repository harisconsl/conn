#ifndef _IN_COMMON_NOTFIERQ_H_
#define _IN_COMMON_NOTIFIERQ_H_
#include <NotifierFd.h>

namespace IN {
namespace COMMON {

template <typename T>
class NotifierQ : public T ,  public NotifierFd
{
 public:
  template < class... Args >
    class NotifierQ(Args&&... args)
    : T(std::forward(args)...)
    { }
};
 
}}//namespace ends
#endif


