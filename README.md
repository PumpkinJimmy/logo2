## 大作业：Logo2
一个大作业，包含一套简易解释器，解释执行一种类似logo的语言，一些绘图前端，允许输出多种格式的图形，以及一个简易的Qt IDE用于编写这一语言并实时查看绘图结果

### 支持的语句
（关键字区分大小写）
set var=val/$var2   #keywd("set") << varname << "=" << val
move len   #keywd("move") << val
turn deg #keywd("turn") << val
color r g b #keywd("color") << val << val << val
cloak on/off(隐身) #keywd("cloak")
print str1 str2 ... #keywd("print") *(val|str)
loop loopcnt #keywd("loop") << *statement << keywd("end") << keywd("loop")
end loop
def func(args...) #keywd("def") << varname << args << *statement << keywd("end") << keywd("def")
end def
call func(args...) #keywd("call") << varname
#func(args...)
#return $var/val
引用变量：“$记法”
不能使用运算符
#所有计算只处理整数
#处理运算符：+-*/%**
变量只保存整数
print可以输出
函数没有返回值

#函数调用可以有返回值，返回值必须是右值


set var val
move len
turn deg
color r g b
print str1 str2 ...
loop loop cnt

endloop
def func arg1 arg2 ...