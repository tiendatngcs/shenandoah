/*
 * Copyright (c) 2020, Red Hat, Inc. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 *
 */

#include "precompiled.hpp"
#include "gc/shenandoah/heuristics/shenandoahAdaptiveHeuristics.hpp"
#include "gc/shenandoah/heuristics/shenandoahAdaptiveOldHeuristics.hpp"
#include "gc/shenandoah/heuristics/shenandoahAggressiveHeuristics.hpp"
#include "gc/shenandoah/heuristics/shenandoahAggressiveOldHeuristics.hpp"
#include "gc/shenandoah/heuristics/shenandoahCompactHeuristics.hpp"
#include "gc/shenandoah/heuristics/shenandoahCompactOldHeuristics.hpp"
#include "gc/shenandoah/heuristics/shenandoahStaticHeuristics.hpp"
#include "gc/shenandoah/heuristics/shenandoahStaticOldHeuristics.hpp"
#include "gc/shenandoah/shenandoahGeneration.hpp"
#include "gc/shenandoah/mode/shenandoahMode.hpp"

ShenandoahHeuristics* ShenandoahMode::initialize_heuristics(ShenandoahGeneration* generation) const {

  if (ShenandoahGCHeuristics != NULL) {
    if (strcmp(ShenandoahGCHeuristics, "aggressive") == 0) {
      return new ShenandoahAggressiveHeuristics(generation);
    } else if (strcmp(ShenandoahGCHeuristics, "static") == 0) {
      return new ShenandoahStaticHeuristics(generation);
    } else if (strcmp(ShenandoahGCHeuristics, "adaptive") == 0) {
      return new ShenandoahAdaptiveHeuristics(generation);
    } else if (strcmp(ShenandoahGCHeuristics, "compact") == 0) {
      return new ShenandoahCompactHeuristics(generation);
    } else {
      vm_exit_during_initialization("Unknown -XX:ShenandoahGCHeuristics option");
    }
  }
  ShouldNotReachHere();
  return NULL;
}

ShenandoahOldHeuristics* ShenandoahMode::initialize_old_heuristics(ShenandoahGeneration* generation) const {

  assert(ShenandoahGCHeuristics != NULL, "ShenandoahGCHeuristics should not equal NULL");
  if (strcmp(ShenandoahGCHeuristics, "aggressive") == 0) {
    return new ShenandoahAggressiveOldHeuristics(generation);
  } else if (strcmp(ShenandoahGCHeuristics, "static") == 0) {
    return new ShenandoahStaticOldHeuristics(generation);
  } else if (strcmp(ShenandoahGCHeuristics, "adaptive") == 0) {
    return new ShenandoahAdaptiveOldHeuristics(generation);
  } else if (strcmp(ShenandoahGCHeuristics, "compact") == 0) {
    return new ShenandoahCompactOldHeuristics(generation);
  } else {
    vm_exit_during_initialization("Unknown -XX:ShenandoahGCHeuristics option");
    ShouldNotReachHere();
    return NULL;
  }
}
