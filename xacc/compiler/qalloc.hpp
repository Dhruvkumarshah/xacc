/*******************************************************************************
 * Copyright (c) 2020 UT-Battelle, LLC.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompanies this
 * distribution. The Eclipse Public License is available at
 * http://www.eclipse.org/legal/epl-v10.html and the Eclipse Distribution
 *License is available at https://eclipse.org/org/documents/edl-v10.php
 *
 * Contributors:
 *   Alexander J. McCaskey - initial API and implementation
 *******************************************************************************/
#ifndef XACC_QALLOC_HPP_
#define XACC_QALLOC_HPP_

#include <map>
#include <string>
#include <memory>

namespace xacc {
class AcceleratorBuffer;
class Observable;
namespace internal_compiler {
using qubit = std::pair<std::string, std::size_t>;
class qreg;
// Classical register associated with a qubit register to store single-shot
// measurement results. i.e. after Measure(q[0]) => q.creg[0] will contain the
// boolean (true/false) result of the measurement.
class cReg {
public:
  cReg() = default;
  cReg(std::shared_ptr<AcceleratorBuffer> in_buffer);
  bool operator[](std::size_t i);

private:
  std::shared_ptr<AcceleratorBuffer> buffer;
};
class qreg {
private:
  std::string random_string(std::size_t length);
protected:
  std::shared_ptr<AcceleratorBuffer> buffer;
  bool been_named_and_stored = false;

public:
  qreg() = default;
  qreg(const int n);
  qreg(const qreg &other);
  qubit operator[](const std::size_t i);
  AcceleratorBuffer *results();
  std::map<std::string, int> counts();
  double exp_val_z();
  void reset();
  int size();
  void addChild(qreg &q);
  void setName(const char *name);
  void setNameAndStore(const char *name);
  std::string name();
  void store();
  void print();
  double weighted_sum(Observable *obs);
  void write_file(const std::string& file_name);
  friend std::ostream& operator<<(std::ostream& os, qreg& q);
  // Public member var to simplify the single measurement syntax:
  // i.e. we can access the single measurement results via the syntax:
  // q.creg[i] vs. (*q.results())[i]
  cReg creg;
};
std::ostream& operator<<(std::ostream& os, qreg& q);
} // namespace internal_compiler
} // namespace xacc

xacc::internal_compiler::qreg qalloc(const int n);

// __qpu__ indicates this functions is for the QCOR Clang Syntax Handler
// and annotated with quantum for the LLVM IR CodeGen
#define __qpu__ [[clang::syntax(qcor)]] __attribute__((annotate("quantum")))

#endif