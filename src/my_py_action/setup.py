from setuptools import setup

package_name = 'my_py_action'

setup(
    name=package_name,
    version='0.0.0',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='ubuntu',
    maintainer_email='2993821152@qq.com',
    description='TODO: Package description',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'my_py_actionclient_node = my_py_action.my_py_actionclient_node:main',
            'my_py_actionserver_node = my_py_action.my_py_actionserver_node:main'
        ],
    },
)
