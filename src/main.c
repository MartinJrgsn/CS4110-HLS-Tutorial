

#include "platform.h"
#include "xfir.h"
#include "xil_printf.h"

int main()
{
    print("\n\rFIR Filter\n\r>");

    XFir fir =
    {
        .Control_BaseAddress = XPAR_XFIR_0_BASEADDR,
        .IsReady = 0
    };

    init_platform();

    XFir_Config* const config = XFir_LookupConfig(XPAR_XSLCR_0_DEVICE_ID);
    const int ret = XFir_CfgInitialize( &fir, config);
    Xil_AssertNonvoid( ret == XST_SUCCESS );

    XFir_DisableAutoRestart(&fir);

    while(1)
    {
        const char c = inbyte();
        if(XFir_IsIdle(&fir))
        {
            XFir_Set_input_r(&fir, c);
            XFir_Start(&fir);

            while(!XFir_IsDone(&fir));

            const uint8_t val = XFir_Get_output_r(&fir);
            outbyte(val);
        }
    }

    cleanup_platform();
    return 0;
}
