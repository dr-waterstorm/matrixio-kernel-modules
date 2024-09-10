#include <linux/bitops.h>
#include <linux/fs.h>
#include <linux/gpio/consumer.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/mutex.h>

#include "matrixio-core.h"

struct matrixio_gpio {
	struct gpio_descs *desc;
	struct matrixio *mio;
	struct mutex lock;
};

static int matrixio_gpio_get_direction(struct gpio_desc *desc)
{
	return gpiod_get_direction(desc);
}

static int matrixio_gpio_direction_input(struct gpio_desc *desc)
{
	return gpiod_direction_input(desc);
}

static int matrixio_gpio_direction_output(struct gpio_desc *desc, int value)
{
	return gpiod_direction_output(desc, value);
}

static int matrixio_gpio_get_value(struct gpio_desc *desc)
{
	return gpiod_get_value(desc);
}

static void matrixio_gpio_set_value(struct gpio_desc *desc, int value)
{
	gpiod_set_value(desc, value);
}

static int matrixio_gpio_probe(struct platform_device *pdev)
{
	struct matrixio_gpio *gpio;
	int ret, i;

	gpio = devm_kzalloc(&pdev->dev, sizeof(*gpio), GFP_KERNEL);
	if (!gpio)
		return -ENOMEM;

	platform_set_drvdata(pdev, gpio);

	gpio->mio = dev_get_drvdata(pdev->dev.parent);
	gpio->desc = gpiod_get_array(&pdev->dev, NULL, GPIOD_ASIS);

	if (IS_ERR(gpio->desc)) {
		dev_err(&pdev->dev, "Failed to get GPIO array\n");
		return PTR_ERR(gpio->desc);
	}

	mutex_init(&gpio->lock);

	for (i = 0; i < gpio->desc->ndescs; i++) {
		ret = matrixio_gpio_direction_input(gpio->desc->desc[i]);
		if (ret) {
			dev_err(&pdev->dev, "Failed to set GPIO %d as input\n", i);
			return ret;
		}
	}

	dev_info(&pdev->dev, "MATRIXIO GPIO driver loaded successfully\n");
	return 0;
}

static int matrixio_gpio_remove(struct platform_device *pdev)
{
	struct matrixio_gpio *gpio = platform_get_drvdata(pdev);

	mutex_destroy(&gpio->lock);
	gpiod_put_array(gpio->desc);

	return 0;
}

static const struct of_device_id matrixio_gpio_of_match[] = {
	{ .compatible = "matrixio,gpio" },
	{},
};
MODULE_DEVICE_TABLE(of, matrixio_gpio_of_match);

static struct platform_driver matrixio_gpio_driver = {
	.driver = {
		.name = "matrixio-gpio",
		.of_match_table = matrixio_gpio_of_match,
	},
	.probe = matrixio_gpio_probe,
	.remove = matrixio_gpio_remove,
};

module_platform_driver(matrixio_gpio_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Andres Calderon <andres.calderon@admobilize.com>");
MODULE_DESCRIPTION("MATRIXIO GPIO module using gpiod");
