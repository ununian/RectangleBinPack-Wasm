# 记一次在 Web 中使用 C++

> C++库：https://github.com/juj/RectangleBinPack
>
> Wasm：https://github.com/ununian/RectangleBinPack-Wasm
>
> Demo：https://github.com/ununian/RectangleBinPack-Wasm-Demo

## 1. 起因

​	事情的起因是因为小学妹的一个小课题，要写一个矩形排样（二维矩形装箱）的程序。具体来说就是需要在规定大小的大矩形中排列若干个大小不一的小矩形，求如何排列小矩形使得空间利用率最高。（具体来说就给定一系列矩形，需要将这些矩形打包到一个给定大小的二维箱子中，且任何两个矩形不能相交和包含，求如何排列矩形使得需要的箱子最少，且利用率最大）这是一个很有现实意义的问题，在工业中非常重要，而且排样的结果往往和经济利益相挂钩。另外这是一个NP-Hard问题，没有一个简单的公式可以计算。

## 2. 思路	

​	鉴于小学妹是不学无术的代表，而我对算法可以说是一窍不通，那么找现成的算法是唯一的办法。经过各种曲折，终于找到了一个叫 RectangleBinPack 的库，而且很贴心的是这个库里面还有一篇介绍二维矩形装箱各种算法的文章，这个库就是这篇文章中各种算法的实现，如果对算法有兴趣的话可以看 Wasm 仓库里面的 pdf，我把英文版和中文机翻版本都放在里面了。

​	现在我们需要了解的是，一共有 4 种算法分别是：货架算法、断头台算法、最大矩形算法、天际线算法。每个算法都有一些策略选项。然后这次毕竟这里只是一个小的课题研究，我们就简化一下问题，只考虑一个箱子的情况。

## 3.方案

​	因为该库是 C++ 写的，我对 C++ 并不是很熟悉，早就还给老师了，所以需要用我熟悉的语言来使用。在其他语言中使用 C++ 有两种方案：

> * 一种是直接改写成对应的语言，这种适用于比较简单的库。其实这个库很适合直接改写，但是那就太没意思了，不能在小学妹前体现我的水平。
> * 第二种是通过将 C++ 库编译成静态库，然后通过跨语言调用机制去直接调用这个静态库。本文将使用这种方式。

​	首先我想到的是 C#，毕竟在 C# 中调用 C++ 是很常见的操作，也有毕竟成熟的 Warp 工具如 Swig，我之前也这样做过。但是使用 C# 会有几个问题，第一是使用起来比较麻烦，毕竟是桌面程序，就有分发、安装、兼容性等问题要处理。第二是编译的结果不跨平台，虽然 C++ 和 C# 本身都能跨平台，但是要针对每个平台都编译一次，而且 C# 的 GUI 部分要跨平台的话写起来会比较麻烦。

​	然后我就想到了 WebAssembly，基本上完美的解决了上面的问题。不用担心跨平台的问题，又能直接使用前端的技术来写 GUI 部分，很方便。然后主要是看起来很高端，能在小学妹面前装逼，那当然就选这个了！

## 4. 实现

### 4.1  环境需求

> * 最好是 Linux，可以省去很多奇奇怪怪的问题，如果是 Windows 可以试试 WSL。
> * 安装好 Emscripten 
>   * 参考 [Download and install — Emscripten 2.0.27 (dev) documentation](https://emscripten.org/docs/getting_started/downloads.html)
> * Node 以及网页开发相关的工具

### 4.2  项目结构

```shell
.
├── XXXX.cpp 		// 算法本身的 cpp 文件
├── XXXX.h			// 算法本身的头文件
├── Warp.cc			// Warp文件，其中描述了需要导出的类和函数、枚举等
├── compile.sh		// 编译脚本
├── package.json	// package.json 文件，方便发布到 npm 仓库		
```

### 4.3  Wrap 文件的编写

​	在 Warp 文件中我们需要显示的告诉 Emscripten 我们需要导出那些类和函数（称之为 Binding），方便 Emscripten 生成相应的代码。让我们来看看本项目的 Warp 是什么样子的

```c++
#include "Rect.cpp"
// ... include<xxx.cpp> 引用各个算法的 cpp 文件
#include "emscripten/bind.h"  	// Emscripten Binding 需要的头文件

using namespace emscripten;		// Emscripten Binding 的命名空间
using namespace std;			// C++ 标准库命名空间 ，主要是为了使用 vector（可以理解为 C++ 中的可变长度数组）
using namespace rbp;			// 这个算法库的命名空间 RectangleBinPack

EMSCRIPTEN_BINDINGS(c)			// 表示我们开始编写 Emscripten 的 Binding
{
    	// 下面只要是字符串里面的值都是在 wasm 里面的名字，可以自己取，不强求和 C++ 中的一样。
    
	// 导出 Rect 和 RectSize 的 vector
    	register_vector<Rect>("VectorRect");
	register_vector<RectSize>("VectorRectSize");

	// Rect.cpp
	class_<RectSize>("RectSize")	// 导出 RectSize 类，他包括
			.constructor<>()		// 一个没有参数的构造函数
			.constructor<int, int>()	// 一个有 2 个参数的构造函数，参数的类型分别是 int 和 int
			.property("width", &RectSize::width)	// 一个实例字段 width，对应的地址是 RectSize 的 width
			.property("height", &RectSize::height);

    
    	 // ...

	emscripten::function("IsContainedIn", &IsContainedIn);	// 导出了一个全局的函数

	// SkylineBinPack.cpp
    	// 导出一个叫做 SkylineBinPack_LevelChoiceHeuristic 的枚举，
    	// 他有 2 个值 LevelBottomLeft、LevelMinWasteFit
	enum_<SkylineBinPack::LevelChoiceHeuristic>("SkylineBinPack_LevelChoiceHeuristic")
			.value("LevelBottomLeft", SkylineBinPack::LevelChoiceHeuristic::LevelBottomLeft)
			.value("LevelMinWasteFit", SkylineBinPack::LevelChoiceHeuristic::LevelMinWasteFit);

	class_<SkylineBinPack>("SkylineBinPack")
			.constructor<>()
			.constructor<int, int, bool>()
			.function("Init", &SkylineBinPack::Init) // 一个实例函数 Init
       		 	// 一个实例函数 Insert_Range，他对应的是 Insert 函数的某个重载
			.function("Insert_Range",select_overload<void(vector<RectSize> &, vector<Rect> &, SkylineBinPack::LevelChoiceHeuristic)>(&SkylineBinPack::Insert)) 
			.function("Insert_Single",select_overload<Rect(int, int, SkylineBinPack::LevelChoiceHeuristic)>(&SkylineBinPack::Insert))
			.function("Occupancy", &SkylineBinPack::Occupancy);
}
```

​	Warp 文件本身和文件中是字符串里面的值都是在 wasm 里面的名字，可以自己取，不强求和 C++ 中的一样。

​	记住直接引入 cpp 文件，不是头文件。下面来说说几个重要的部分

#### 4.3.1 vector 的处理

​	vector 是 C++ 标准库中提供的一个数据结构，是一个可以动态改变长度的数组。本项目中主要用来传递需要排版的 RectSize 数组 以及接收计算结果 Rect 数组。因为 vector 是很常用的数据结构，所以 Emscripten 很贴心的帮我们提供了自动绑定的方法 register_vector ，只需要传入 vector 的元素类型和导出的名字就好了。

#### 4.3.2 枚举的处理

​	JS 中没有枚举的概念，所以在 JS 使用的时候是用 Object 的形式。绑定也很简单，使用 enum_ 指定名称和类型，以及对应的值就行。

#### 4.3.3 函数重载的处理

​	因为 JS 中没有函数重载的概念，所以导出重载函数需要指定不同的名称，并使用 select_overload 函数找到对应的函数（ 指定函数的返回值、参数的类型就行，没有返回值就是 void ）。顺带一提如果有多个构造函数也需要指定构造函数的参数类型（ 构造函数不能指定名称和返回值 ）。

### 4.4 编译 Wasm

​	接下来我们需要把写好的 Warp 文件编译成 Wasm，编译脚本如下

```shell
emcc --bind -Oz Warp.cc -o dist/Warp.js \
-s WASM=1 \
-s MODULARIZE=1
```

> --bind 表示需要使用 Embind 的绑定功能。
>
> -Oz 表示优化等级，有O0、O1、O2 等等，Oz表示优化等级最高，因为我们不需要调试这个Wasm，所以选 Oz就行。
>
> -o 指定输出文件，这里需要注意的是，如果指定的文件后缀名是 js，那么就会生成 wasm 和 响应的 js warp 文件，这个 js warp 文件主要用于是方便我们使用这个库。也可以指定 html 生成一个 demo 网页，指定 wasm 只生成 wasm 文件。
>
> -s WASM=1 表示编译到 wasm ，如果是 0 就会编译到 asm.js，2 就同时编译成两者。
>
> -s MODULARIZE=1 表示生成的 js 文件会导出一个可以传参工厂函数（后续会看到），否则会直接赋值在 window 对象上。
>
> 另外还有一个值得一说的是 -s SINGLE_FILE=1，它可以用 base64 的方式把 wasm 嵌入到 warpjs 文件中，用的时候只需要引用 js 文件就行。

### 4.5 生成对应的 TypeScript 描述文件

> 工具地址：[ted537/tsembind: Generate typescript bindings for Embind projects (github.com)](https://github.com/ted537/tsembind)

​	为了方便在工程中使用，生成 TypeScript 的描述文件是很重要的一步，不然别人用的时候根本不知道怎么用（ 还能减少写文档的工作量 ）。

​	不过当前并没有很完善的方案处理这个问题，我选的这个工具是读取 wasm 文件分析里面的导出，所以不能知道函数的形参名字，然后生成的描述文件还有一点点我们需要改动的地方。

​	直接运行 ```tsembind ./dist/Warp.js > ./dist/Warp.d.ts``` 就行了，然后修改一下最下面导出的部分，记得添加 `@types/emscripten`

```tsx
export interface CustomEmbindModule { 
	// ...
}
declare function factory(): Promise<CustomEmbindModule>;
export default factory;

// =========>

export interface RectangleBinPackModule extends EmscriptenModule {
	// ...
}
declare const factory: EmscriptenModuleFactory<RectangleBinPackModule>;
export default factory;
```

### 4.6 使用 Wasm

详细的请参考 Demo 仓库中的代码，效果如下：

![10](https://user-images.githubusercontent.com/4506072/132728723-94fad3cf-4dfc-418d-82ea-b875a16cf7e6.gif)


下面说一些要注意的地方

```tsx
import type { RectangleBinPackModule as PackModule } from 'rectanglebinpack-wasm'

// PackWasmInit 就是上面那个工厂函数
import PackWasmInit from 'rectanglebinpack-wasm';

// 我们需要获取 wasm 文件的路径。我们不需要用打包器的 wasm loader，
// 只需要这个wasm文件的 url 就行。这里是 vite 的写法，webpack 应该是 file-loader
import PackWasm from 'rectanglebinpack-wasm/dist/Warp.wasm?url' 

// 方便获取枚举的值，主要是用来规避 ts 的类型检查
const toEnumValue = (enumObj: any, value: any) => enumObj[value]

export class WasmPackService implements IPackService {

  private wasm?: PackModule;

  constructor() {
    PackWasmInit({ 
    	// 这里非常重要，我们需要告诉工厂方法 wasm 文件的位置在哪，如果不写的话，他会去网页的根目录下查找，一般情况下我们不希望这样
        locateFile: (url) => url.endsWith('.wasm') ? PackWasm : url  
    }).then(wasm => {
      this.wasm = wasm;	// 初始化完成后，就能获取到 wasm 模块的对象了
    })
  }
    
  public async pack(
    source: SourcePanelItem[], // width height 这里因为只考虑 1 个箱子的情况，所以这里肯定只有 1 个数据
    target: TargetPanelItem[], // width height count
    algorithms: Algorithms,	// 算法
    setting: Record<string, boolean | string> // 算法设置
  ) {
      // ...
      const m = this.wasm;
      
      // 首先我们创建一个 RectSize 的 vector，然后把我们需要排版的小矩形都放进去
      const targetSizes = new m.VectorRectSize();	
      target
        .flatMap(t => range(0, Math.max(t.count, 0))
          .map(_ => new m.RectSize(t.width, t.height)))
        .forEach((i) => targetSizes.push_back(i));

      // ...
      let resultRects = new m.VectorRect();	// 创建一个用来接收结果的 Rect 的 vector
      switch (algorithms) {
      	// ...
          case "Skyline":
              // 调用天际线算法类的构造函数，并传递一些设置，创建一个算法对象
              const skyline = new m.SkylineBinPack(sourceWidth, sourceHeight, setting['UseWasteMap'] as boolean);
              // 调用批量添加函数，函数内部会把结果添加到 resultRects 里面
              skyline.Insert_Range(
                targetSizes,
                resultRects,
                toEnumValue(m.SkylineBinPack_LevelChoiceHeuristic, setting['LevelChoiceHeuristic'])
              );
              // 重要：手动释放 skyline 对象。因为 wasm 需要我们手动管理内存，所以创建了对象后一定要回收，不存在自动垃圾回收。
              skyline.delete();
              break;
      }
      const result: Rect[] = []
      for (let i = 0; i < resultRects.size(); i++) {
        const item = resultRects.get(i);
        result.push({ x: item.x, y: item.y, width: item.width, height: item.height })
      }
      // 获取结果后释放掉 targetSizes、resultRects
      targetSizes.delete();
      resultRects.delete();
        
      return { result }
  }
}
```

#### 4.6.1 内存管理

​	在使用 Wasm 中和 JS 差别最大的一点就是对象的内存都是手动创建（ new 函数 ）和释放的（ delete 函数 ）。请注意 new 和 delete 的成对使用。 vector 内如果存的不是指针，则会自动调用析构函数。

#### 4.6.2 指定 Wasm 文件的 Url

​	如果不指定 wasm 文件的 Url，那么 warp 文件会从网站根目录/xx.wasm 来加载，通常我们是不希望这样的，所以我们需要在 wasm 加载时通过 `locateFile`函数指定 wasm 文件的 Url。记住不要通过 webpack 或者 vite 的 loader 来加载 wasm，那样会自动转换成 wasm 模块，我们只需要获取 wasm 文件的 url 就行，在vite 中实在资源名后加上 `?url` ，webpack 中是加上 `!file-loader`。



## 5.总结

​	这次尝试涉及到的东西还是挺多的，有C++、编译器、WebAssembly 、loader。整体做下来踩得坑也挺多的，主要是这方面的资料比较少，很多要么很简单就是导出一个全局函数，要么就是很复杂的一个完整项目如 ffmpeg 的 wasm 版本。之前一直想学习 WebAssembly，这次总算有次机会了比较简单的了解了从编译到使用的全过程。

​	最后做出来的效果还比较满意，也有一定的实际运用价值，当然主要是小学妹也很满意。

## 6.彩蛋 ：C# 的做法

### 6.1 编写描述文件 Warp.idl

 ```idl
 %module RectangleBinPack
 
 %{
 #include "Rect.h"
 #include "GuillotineBinPack.h"
 #include "SkylineBinPack.h"
 #include "ShelfNextFitBinPack.h"
 #include "ShelfBinPack.h"
 #include "MaxRectsBinPack.h"
 %}
 
 %include <std_vector.i>
 %template(vector_Rect) std::vector<rbp::Rect>;
 %template(vector_RectSize) std::vector<rbp::RectSize>;
 
 %include "Rect.h"
 %include "GuillotineBinPack.h"
 %include "SkylineBinPack.h"
 %include "ShelfNextFitBinPack.h"
 %include "ShelfBinPack.h"
 %include "MaxRectsBinPack.h"
 ```

确实比 Emscripten 方便很多，毕竟成熟很多。再调用 

```shell
swig -c++ -csharp Warp.idl
```

这一步会生成很多 cs 文件（ C#的源文件 ）和一个 warp.cxx 文件

### 6.2 编译 Dll

​	比较幸运的是，RectangleBinPack 自带了 VisualStudio 的工程文件  RectangleBinPack.sln ，打开后将上面生成的 warp.cxx 文件加入工程就行，然后 build 一个 x64 的版本。

### 6.3 使用

​	创建一个C# GUI项目，将 6.1 生成的的 cs 文件和 6.2 生成的 dll 复制到目录下，dll 记得选择较新则复制。下面将比较重要的代码贴一下：

```csharp
  public (double, List<Rect>) PackImplement(PackRequestDto dto)
        {
      			// ...
                var targets = new vector_RectSize(dto.Target.SelectMany(target =>
                    Enumerable.Range(1, target.Count).Select(_ =>
                        new RectSize {width = target.Width, height = target.Height})));

                var resultRects = new vector_Rect();
              
      			// ...

                switch (dto.Algorithms)
                {
                    case PackAlgorithms.Skyline:
                        var skylineBin = new SkylineBinPack(sourceWidth, sourceHeight,
                            dto.SkylineSetting.UseWasteMap);
                        skylineBin.Insert(targets, resultRects, dto.SkylineSetting.LevelChoiceHeuristic);
                       
                        break;
                   // ...
                }
                return (occupancy, resultRects.ToList());
            }
```

可以看出来，C# 和 JS 在调用阶段都差不多，只是 swig 比较贴心的自己处理了内存管理部分。

