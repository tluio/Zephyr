/*
 * Copyright (c) 2010, 2012-2015 Wind River Systems, Inc.
 * Copyright (c) 2019 Intel Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * This file is a template for cmake and is not meant to be used directly!
 */

#ifdef CONFIG_UART_INTERRUPT_DRIVEN
static void irq_config_func_1(struct device *port);
#endif

static const struct uart_ns16550_device_config uart_ns16550_dev_cfg_1 = {
	.sys_clk_freq = DT_UART_NS16550_PORT_1_CLK_FREQ,

#ifdef CONFIG_UART_INTERRUPT_DRIVEN
	.irq_config_func = irq_config_func_1,
#endif

#ifdef DT_UART_NS16550_PORT_1_PCP
	.pcp = DT_UART_NS16550_PORT_1_PCP,
#endif

#if DT_UART_NS16550_PORT_1_PCIE
	.pcie = true,
	.pcie_bdf = DT_UART_NS16550_PORT_1_BASE_ADDR,
	.pcie_id = DT_UART_NS16550_PORT_1_SIZE,
#endif
};

static struct uart_ns16550_dev_data_t uart_ns16550_dev_data_1 = {
	.port = DT_UART_NS16550_PORT_1_BASE_ADDR,
	.baud_rate = DT_UART_NS16550_PORT_1_BAUD_RATE,
	.options = CONFIG_UART_NS16550_PORT_1_OPTIONS,

#ifdef DT_UART_NS16550_PORT_1_DLF
	.dlf = DT_UART_NS16550_PORT_1_DLF,
#endif
};

DEVICE_AND_API_INIT(uart_ns16550_1, DT_UART_NS16550_PORT_1_NAME,
		    &uart_ns16550_init,
		    &uart_ns16550_dev_data_1, &uart_ns16550_dev_cfg_1,
		    PRE_KERNEL_1, CONFIG_KERNEL_INIT_PRIORITY_DEVICE,
		    &uart_ns16550_driver_api);

#ifdef CONFIG_UART_INTERRUPT_DRIVEN
static void irq_config_func_1(struct device *dev)
{
	ARG_UNUSED(dev);

#if DT_UART_NS16550_PORT_1_PCIE
#if DT_UART_NS16550_PORT_1_IRQ == PCIE_IRQ_DETECT

	/* PCI(e) with auto IRQ detection */

	BUILD_ASSERT_MSG(IS_ENABLED(CONFIG_DYNAMIC_INTERRUPTS),
		"NS16550 PCI auto-IRQ needs CONFIG_DYNAMIC_INTERRUPTS");

	unsigned int irq;

	irq = pcie_wired_irq(DT_UART_NS16550_PORT_1_BASE_ADDR);

	if (irq == PCIE_CONF_INTR_IRQ_NONE) {
		return;
	}

	irq_connect_dynamic(irq,
			    DT_UART_NS16550_PORT_1_IRQ_PRI,
			    uart_ns16550_isr,
			    DEVICE_GET(uart_ns16550_1),
			    DT_UART_NS16550_PORT_1_IRQ_FLAGS);

	pcie_irq_enable(DT_UART_NS16550_PORT_1_BASE_ADDR, irq);

#else

	/* PCI(e) with fixed or MSI IRQ */

	IRQ_CONNECT(DT_UART_NS16550_PORT_1_IRQ,
		    DT_UART_NS16550_PORT_1_IRQ_PRI,
		    uart_ns16550_isr, DEVICE_GET(uart_ns16550_1),
		    DT_UART_NS16550_PORT_1_IRQ_FLAGS);

	pcie_irq_enable(DT_UART_NS16550_PORT_1_BASE_ADDR,
			DT_UART_NS16550_PORT_1_IRQ);

#endif
#else

	/* not PCI(e) */

	IRQ_CONNECT(DT_UART_NS16550_PORT_1_IRQ,
		    DT_UART_NS16550_PORT_1_IRQ_PRI,
		    uart_ns16550_isr, DEVICE_GET(uart_ns16550_1),
		    DT_UART_NS16550_PORT_1_IRQ_FLAGS);

	irq_enable(DT_UART_NS16550_PORT_1_IRQ);

#endif
}
#endif
