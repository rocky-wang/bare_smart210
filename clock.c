#include "clock.h"
#include "cpu_base.h"
#include "debug.h"
/* s5pv210: return pll clock frequency */
static unsigned long s5pv210_get_pll_clk(int pllreg)
{
	struct s5pv210_clock *clk = (struct s5pv210_clock *)S5PV210_CLOCK_BASE;
	unsigned long r, m, p, s, mask, fout;
	unsigned int freq;

	switch (pllreg) {
	case APLL:
		r = readl(&clk->apll_con);
		break;
	case MPLL:
		r = readl(&clk->mpll_con);
		break;
	case EPLL:
		r = readl(&clk->epll_con);
		break;
	case VPLL:
		r = readl(&clk->vpll_con);
		break;
	default:
		INFO("Unsupported PLL (%d)\n", pllreg);
		return 0;
	}

	/*
	 * APLL_CON: MIDV [25:16]
	 * MPLL_CON: MIDV [25:16]
	 * EPLL_CON: MIDV [24:16]
	 * VPLL_CON: MIDV [24:16]
	 */
	if (pllreg == APLL || pllreg == MPLL)
		mask = 0x3ff;
	else
		mask = 0x1ff;

	m = (r >> 16) & mask;

	/* PDIV [13:8] */
	p = (r >> 8) & 0x3f;
	/* SDIV [2:0] */
	s = r & 0x7;

	freq = CONFIG_SYS_CLK_FREQ_C110;
	if (pllreg == APLL) {
		if (s < 1)
			s = 1;
		/* FOUT = MDIV * FIN / (PDIV * 2^(SDIV - 1)) */
		fout = m * (freq / (p * (1 << (s - 1))));
	} else
		/* FOUT = MDIV * FIN / (PDIV * 2^SDIV) */
		fout = m * (freq / (p * (1 << s)));

	return fout;
}

unsigned long get_pll_clk(int pllreg)
{
    return s5pv210_get_pll_clk(pllreg);
}


/* s5pv210: return ARM clock frequency */
static unsigned long s5pv210_get_arm_clk(void)
{
	struct s5pv210_clock *clk =(struct s5pv210_clock *)S5PV210_CLOCK_BASE;
	unsigned long div;
	unsigned long dout_apll, armclk;
	unsigned int apll_ratio;

	div = readl(&clk->div0);

	/* APLL_RATIO: [2:0] */
	apll_ratio = div & 0x7;

	dout_apll = get_pll_clk(APLL) / (apll_ratio + 1);
	armclk = dout_apll;

	return armclk;
}

unsigned long get_arm_clk(void)
{
	return s5pv210_get_arm_clk();
}

static unsigned long get_hclk(void)
{
	struct s5pv210_clock *clk =(struct s5pv210_clock *)S5PV210_CLOCK_BASE;
	unsigned long hclkd0;
	unsigned int div, d0_bus_ratio;

	div = readl(&clk->div0);
	/* D0_BUS_RATIO: [10:8] */
	d0_bus_ratio = (div >> 8) & 0x7;

	hclkd0 = get_arm_clk() / (d0_bus_ratio + 1);

	return hclkd0;
}


static unsigned long get_hclk_sys(int dom)
{
	struct s5pv210_clock *clk =(struct s5pv210_clock *)S5PV210_CLOCK_BASE;
	unsigned long hclk;
	unsigned int div;
	unsigned int offset;
	unsigned int hclk_sys_ratio;

	if (dom == CLK_M)
		return get_hclk();

	div = readl(&clk->div0);

	/*
	 * HCLK_MSYS_RATIO: [10:8]
	 * HCLK_DSYS_RATIO: [19:16]
	 * HCLK_PSYS_RATIO: [27:24]
	 */
	offset = 8 + (dom << 0x3);

	hclk_sys_ratio = (div >> offset) & 0xf;

	hclk = get_pll_clk(MPLL) / (hclk_sys_ratio + 1);

	return hclk;
}

static unsigned long get_pclk_sys(int dom)
{
	struct s5pv210_clock *clk =(struct s5pv210_clock *)S5PV210_CLOCK_BASE;
	unsigned long pclk;
	unsigned int div;
	unsigned int offset;
	unsigned int pclk_sys_ratio;

	div = readl(&clk->div0);

	/*
	 * PCLK_MSYS_RATIO: [14:12]
	 * PCLK_DSYS_RATIO: [22:20]
	 * PCLK_PSYS_RATIO: [30:28]
	 */
	offset = 12 + (dom << 0x3);

	pclk_sys_ratio = (div >> offset) & 0x7;

	pclk = get_hclk_sys(dom) / (pclk_sys_ratio + 1);

	return pclk;
}


static unsigned long s5pv210_get_pclk(void)
{
	return get_pclk_sys(CLK_P);
}

static unsigned long s5pv210_get_uart_clk(int dev_index)
{
    return s5pv210_get_pclk();
}

unsigned long get_uart_clk(int dev_index)
{
	return s5pv210_get_uart_clk(dev_index);
}


