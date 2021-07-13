#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
   // constructor
  float Utilization();  // See src/processor.cpp

  // Declare any necessary private members
 private:
  long idle_, nonidle_;
};

#endif