#ifndef _IN_MACROS_H_
#define _IN_MACROS_H_


#define PREVENT_COPY(type)                      \
  type(type&) = delete;                         \
  type(const type&) = delete;                   \
  type& operator = (type&) = delete;            \
  type& operator = (const type& ) = delete;	

#define PREVENT_MOVE(type)                      \
  type(type&&) = delete;                        \
  type& operator = (type&&) = delete;           

#define PREVENT_COPY_MOVE(type)                 \
  PREVENT_COPY(type)                         
  PREVENT_MOVE(type)

#endif
