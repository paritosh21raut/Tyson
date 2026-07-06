from setuptools import setup

package_name = "tyson_odometry"

setup(
    name=package_name,
    version="0.0.0",
    packages=[package_name],

    data_files=[
        ("share/ament_index/resource_index/packages",
            ["resource/" + package_name]),

        ("share/" + package_name,
            ["package.xml"]),

        ("share/" + package_name + "/launch",
            ["launch/odometry.launch.py"]),
    ],

    install_requires=["setuptools"],

    zip_safe=True,

    maintainer="paritosh",

    maintainer_email="paritosh@example.com",

    description="Tyson Odometry",

    license="Apache-2.0",

    tests_require=["pytest"],

    entry_points={
        "console_scripts": [
            "odometry_node = tyson_odometry.odometry_node:main",
        ],
    },
)