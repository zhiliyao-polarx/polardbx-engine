/* Copyright (c) 2018, 2021, Alibaba and/or its affiliates. All rights reserved.
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License, version 2.0,
   as published by the Free Software Foundation.
   This program is also distributed with certain software (including
   but not limited to OpenSSL) that is licensed under separate terms,
   as designated in a particular file or component or in included license
   documentation.  The authors of MySQL/PolarDB-X Engine hereby grant you an
   additional permission to link the program and your derivative works with the
   separately licensed software that they have included with
   MySQL/PolarDB-X Engine.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License, version 2.0, for more details.
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA */

#include "trx0purge.h"

#include "lizard0gcs.h"
#include "lizard0undo.h"

#include "lizard_iface.h"

#include <sql_class.h>

my_gcn_t innobase_load_gcn() {
  return static_cast<my_gcn_t>(lizard::gcs_load_gcn());
}

my_scn_t innobase_load_scn() {
  return static_cast<my_scn_t>(lizard::gcs_load_scn());
}

bool innobase_snapshot_scn_too_old(my_scn_t scn) {
  return scn < purge_sys->purged_scn.load();
}

bool innobase_snapshot_automatic_gcn_too_old(my_gcn_t gcn) {
  return gcn < purge_sys->purged_gcn.get();
}

bool innobase_snapshot_assigned_gcn_too_old(my_gcn_t gcn) {
  return gcn <= purge_sys->purged_gcn.get();
}

/**
  Initialize innobase extension.

  param[in]  innobase_hton  handlerton of innobase.
*/
void innobase_init_ext(handlerton *innobase_hton) {
  innobase_hton->ext.load_gcn = innobase_load_gcn;
  innobase_hton->ext.load_scn = innobase_load_scn;

  innobase_hton->ext.snapshot_scn_too_old = innobase_snapshot_scn_too_old;
  innobase_hton->ext.snapshot_assigned_gcn_too_old =
      innobase_snapshot_assigned_gcn_too_old;
  innobase_hton->ext.snapshot_automatic_gcn_too_old =
      innobase_snapshot_automatic_gcn_too_old;
}

/**
  Compare whether the xid in thd is the same as the xid in trx (and aslo in
  undo_ptr).

  @params[in]   thd   THD
  @params[in]   trx   trx_t

  @retval true if the xid in thd is the same as the xid in trx.
*/
bool xa_compare_xid_between_thd_and_trx(const THD *thd, const trx_t *trx) {
  XID xid_in_thd;
  thd_get_xid(thd, (MYSQL_XID *)(&xid_in_thd));

  ut_ad(trx_is_registered_for_2pc(trx));

  if (thd->get_transaction()->xid_state()->check_in_xa(false)) {
    if (lizard::trx_is_txn_rseg_updated(trx)) {
      ut_a(!trx->internal);
      ut_a(trx->mysql_thd == thd);
      ut_a(!trx->rsegs.m_txn.xid_for_hash.is_null());
      ut_a(trx->xid->eq(&trx->rsegs.m_txn.xid_for_hash));
      ut_ad(trx->xid->eq(&xid_in_thd));
    }
  }

  return true;
}
