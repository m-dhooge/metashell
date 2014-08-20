#ifndef METASHELL_METADEBUGGER_SHELL_HPP
#define METASHELL_METADEBUGGER_SHELL_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Andras Kucsma (andras.kucsma@gmail.com)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <string>

#include <boost/optional.hpp>

#include <just/console.hpp>

#include <metashell/config.hpp>
#include <metashell/environment.hpp>
#include <metashell/metaprogram.hpp>

namespace metashell {

class metadebugger_shell {
public:
  typedef boost::optional<just::console::color> optional_color;

  metadebugger_shell(
      const config& conf,
      environment& env);

  virtual ~metadebugger_shell();

  virtual void run() = 0;

  virtual void add_history(const std::string& str) = 0;

  virtual void display(
      const std::string& str,
      optional_color color = boost::none) const = 0;

  virtual unsigned width() const = 0;

protected:
  // breakpoint is simply a regex for now
  typedef std::string breakpoint_t;
  typedef std::vector<breakpoint_t> breakpoints_t;

  std::string prompt() const;
  bool stopped() const;
  void line_available(const std::string& line);

  void run_metaprogram_with_templight(const std::string& str);
  void run_metaprogram(const std::string& str);

  void continue_metaprogram();

  void display_current_frame() const;
  void display_forward_trace(const std::string& root_type = "<root>") const;
  void display_current_forward_trace() const;
  void display_back_trace() const;

  const config& conf;
  environment& env;

  metaprogram mp;
  breakpoints_t breakpoints;

  std::string prev_line;
  bool is_stopped;

private:
  // Helpers for display_forward_trace
  typedef std::pair<
      std::string::const_iterator,
      std::string::const_iterator
    > string_range;

  string_range find_type_emphasize(const std::string& type) const;

  void display_range(
      std::string::const_iterator begin,
      std::string::const_iterator end,
      optional_color c) const;

  void display_trace_content(
      string_range range,
      string_range emphasize) const;

  void display_trace_graph(
      unsigned depth,
      const std::vector<unsigned>& depth_counter,
      bool print_mark) const;

  void display_trace_line(
      metaprogram::vertex_descriptor vertex,
      unsigned depth,
      const std::vector<unsigned>& depth_counter,
      const boost::optional<instantiation_kind>& kind,
      unsigned width) const;

  void display_trace_visit(
      metaprogram::vertex_descriptor root_vertex,
      metaprogram::discovered_t& discovered,
      unsigned width) const;

  void display_frame(const metaprogram::frame& frame) const;

  const static std::vector<just::console::color> colors;
};

}

#endif