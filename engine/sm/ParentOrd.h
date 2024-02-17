#ifndef _INTRADE_PARENTORD_H_
#define _INTRADE_PARENTORD_H_

namespace IN{
namespace ENGINE{

class ParentOrd
{
enum State
  {
    Active,
    Cancelled,
    Error,
    PError,
    Completed,
    PartialCompleted
  };

};
}}
#endif
