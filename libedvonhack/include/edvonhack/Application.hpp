
#pragma once

namespace ed {
   class Application {
   public:
      virtual ~Application() = 0;
      virtual bool init() = 0;
      virtual bool onFrame() = 0;
   };

   typedef Application* (*ApplicationCreatorFunction)();
}