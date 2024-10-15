整体介绍如下：

**一、服务器启动与请求处理**

1. 服务器（Server类）启动时，在其构造函数中进行 Windows 平台的网络初始化设置，然后在start方法中创建事件基础对象、HTTP 对象，并设置路由。
   - 设置路由时，将不同的 API 端点（如`/api/controls`、`/api/control/add`等）与相应的处理函数关联起来。这些处理函数在接收到 HTTP 请求时，会解析请求参数，并调用调度器（`Scheduler`类）的相应方法来执行具体操作。

**二、调度器与服务器的交互**

1. Server类在处理 HTTP 请求时，依赖Scheduler类来执行与控制任务相关的操作。
   - 例如，当处理添加控制任务的请求时，`Server`类会调用`Scheduler`的`apiControlAdd`方法。这个方法会检查任务是否已在运行、是否超过工作线程并发限制等条件，如果满足条件，会创建一个新的工作线程（`Worker`类实例）来执行控制任务。
   - 对于获取控制信息的请求，`Server`类调用`Scheduler`的`apiControls`方法，该方法会遍历工作线程映射，将所有正在运行的控制任务信息返回给`Server`，然后`Server`将这些信息以 JSON 格式返回给客户端。

**三、调度器与工作线程的交互**

1. Scheduler作为调度中心，管理着工作线程（Worker）。
   - 在初始化阶段，`Scheduler`的`initAlgorithm`方法根据配置文件中的算法引擎参数初始化算法模型，创建`AlgorithmOpenVINO`或`AlgorithmTensorRT`对象，这些算法对象可供工作线程在执行控制任务时使用。
   - 当有新的控制任务添加时，`Scheduler`创建一个新的`Worker`实例，并通过`addWorker`方法将其添加到工作线程映射中。同时，工作线程在构造函数中接收一个指向`Scheduler`的指针，以便在执行任务过程中与`Scheduler`进行交互，例如获取算法模型（通过`gainAlgorithm`和`giveBackAlgorithm`方法）。
   - 当需要取消控制任务时，`Server`调用`Scheduler`的`apiControlCancel`方法，该方法会从工作线程映射中移除对应的`Worker`实例，并等待其资源被释放。

**四、工作线程与其他模块的交互**

1. `Worker`类与控制任务（`Control`类）：工作线程持有一个指向`Control`类的指针，用于执行具体的控制任务。`Control`类包含了任务的相关参数，如代码、流地址、算法代码、对象代码等，工作线程根据这些参数来执行特定的任务操作。
2. `Worker`类与算法模型：工作线程在执行控制任务过程中，向`Scheduler`请求算法模型进行算法处理。根据`Scheduler`返回的算法对象（可能是`AlgorithmOpenVINO`或`AlgorithmTensorRT`实例），调用相应的算法方法来处理任务数据。
3. `Worker`类与拉取流（`Pullstream`类）和推送流（`Pushstream`类）：工作线程使用`Pullstream`类拉取输入流，根据`Control`类中的配置信息确定数据源。处理完数据后，使用`Pushstream`类推送处理后的输出流，同样根据`Control`类中的配置信息确定推送目标地址。
4. `Worker`类与报警处理：如果在执行控制任务过程中发生报警事件，工作线程会创建一个`Alarm`对象并传递给`Scheduler`进行处理。

**五、报警处理的交互**

1. 工作线程在执行控制任务时，如果发生报警事件，会创建一个Alarm对象并传递给Scheduler
   - `Scheduler`将报警对象添加到报警队列（`mAlarmQ`）中。
   - `Scheduler`的处理报警线程（由`mLoopAlarmThread`指向）会不断从报警队列中获取报警对象，并调用生成报警视频的函数进行处理。
