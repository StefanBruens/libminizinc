/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */

/*
 *  Main authors:
 *     Guido Tack <guido.tack@monash.edu>
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __MINIZINC_FLATTEN_HH__
#define __MINIZINC_FLATTEN_HH__

#include <minizinc/model.hh>
#include <minizinc/astexception.hh>

namespace MiniZinc {

  /// Exception thrown for errors during flattening
  class FlatteningError : public LocationException {
  public:
    FlatteningError(EnvI& env, const Location& loc, const std::string& msg);
    ~FlatteningError(void) throw() {}
    virtual const char* what(void) const throw() {
      return "MiniZinc: flattening error";
    }
  };

  /// Options for the flattener
  struct FlatteningOptions {
    /// Keep output in resulting flat model
    bool keepOutputInFzn;
    /// Verbose output during flattening
    bool verbose;
    /// Only use paths for variables introduced by file 0 (the MiniZinc model)
    bool only_toplevel_paths;
    /// Keep mzn_path annotations in FlatZinc
    bool keep_mzn_paths;
    /// Only range domains for old linearization. Set from redefs to true if not here
    bool onlyRangeDomains;
    /// Create JSON output
    enum OutputMode {
      OUTPUT_ITEM, OUTPUT_DZN, OUTPUT_JSON
    } outputMode;
    /// Default constructor
    FlatteningOptions(void)
    : keepOutputInFzn(false), verbose(false), only_toplevel_paths(false), keep_mzn_paths(false), onlyRangeDomains(false), outputMode(OUTPUT_ITEM) {}
  };

  class Pass {
    public:
      Pass() {};
      virtual Env* run(Env* env) = 0;
      virtual ~Pass() {};
  };

  /// Flatten model \a m several times and record information in its env
  Env* multiPassFlatten(Env& m, std::vector<Pass*>& passes);
  
  /// Flatten model \a m
  void flatten(Env& m, FlatteningOptions opt = FlatteningOptions());

  /// Translate \a m into old FlatZinc syntax
  void oldflatzinc(Env& m);

  /// Populate FlatZinc output model
  void populateOutput(Env& e);
  
  /// Statistics on flat models
  struct FlatModelStatistics {
    /// Number of integer variables
    int n_int_vars;
    /// Number of bool variables
    int n_bool_vars;
    /// Number of float variables
    int n_float_vars;
    /// Number of set variables
    int n_set_vars;
    /// Number of bool constraints
    int n_bool_ct;
    /// Number of integer constraints
    int n_int_ct;
    /// Number of float constraints
    int n_float_ct;
    /// Number of set constraints
    int n_set_ct;
    /// Constructor
    FlatModelStatistics(void)
    : n_int_vars(0), n_bool_vars(0), n_float_vars(0), n_set_vars(0),
      n_bool_ct(0), n_int_ct(0), n_float_ct(0), n_set_ct(0) {}
  };
  
  /// Compute statistics for flat model in \a m
  FlatModelStatistics statistics(Env& m);
  
}

#endif
