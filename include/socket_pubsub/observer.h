/*
 * Copyright (c) 2022, VinAI. All rights reserved. All information
 * information contained herein is proprietary and confidential to VinAI.
 * Any use, reproduction, or disclosure without the written permission
 * of VinAI is prohibited.
 */

#ifndef OBSERVER_H
#define OBSERVER_H

class Observer {
public:
  virtual void Update(void* data) = 0;
};

#endif /* OBSERVER_H */
