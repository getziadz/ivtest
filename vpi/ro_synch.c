# include  <vpi_user.h>
# include  <stdlib.h>
# include  <assert.h>

struct poke_details {
      vpiHandle dst;
      int val;
      int dly;
};

static PLI_INT32 delayed_poke(p_cb_data cb_data)
{
      s_vpi_value value;
      s_vpi_time time;
      struct poke_details*poke = (struct poke_details*)cb_data->user_data;

      value.format = vpiIntVal;
      value.value.integer = poke->val;
      if (poke->dly < 0) {
            vpi_put_value(poke->dst, &value, 0, vpiNoDelay);
      } else {
            time.type = vpiSimTime;
            time.high = 0;
            time.low  = poke->dly;
            time.real = 0.0;
            vpi_put_value(poke->dst, &value, &time, vpiTransportDelay);
      }
      fflush(stderr); // for Windows
      free(poke);
      return 0;
}

static PLI_INT32 poke_compiletf(char*xx)
{
      (void)xx;  /* Parameter is not used. */
      return 0;
}

static PLI_INT32 poke_calltf(char*xx)
{
      s_vpi_value value;
      s_vpi_time poke_time;
      s_cb_data cb_data;
      vpiHandle sys = vpi_handle(vpiSysTfCall, 0);
      vpiHandle argv = vpi_iterate(vpiArgument, sys);
      vpiHandle dst, val, dly, tmp;

      struct poke_details*poke;

      (void)xx;  /* Parameter is not used. */

      assert(argv);

      dst = vpi_scan(argv);
      assert(dst);

      val = vpi_scan(argv);
      assert(val);

      dly = vpi_scan(argv);
      assert(dly);

      tmp = vpi_scan(argv);
      assert(tmp == 0);

      poke = calloc(1, sizeof (struct poke_details));
      assert(poke);

      poke->dst = dst;

      value.format = vpiIntVal;
      vpi_get_value(val, &value);
      poke->val = value.value.integer;

      value.format = vpiIntVal;
      vpi_get_value(dly, &value);
      poke->dly = value.value.integer;

      poke_time.low = 0;
      poke_time.high = 0;
      poke_time.type = vpiSimTime;

      cb_data.reason = cbReadOnlySynch;
      cb_data.cb_rtn = delayed_poke;
      cb_data.user_data = (char*)poke;
      cb_data.time = &poke_time;
      vpi_register_cb(&cb_data);

      return 0;
}

static void poke_after_delay_register(void)
{
      s_vpi_systf_data tf_data;

      tf_data.type      = vpiSysTask;
      tf_data.tfname    = "$delayed_poke";
      tf_data.calltf    = poke_calltf;
      tf_data.compiletf = poke_compiletf;
      tf_data.sizetf    = 0;
      vpi_register_systf(&tf_data);

}

void (*vlog_startup_routines[])(void) = {
      poke_after_delay_register,
      0
};
