window.__require = function e(t, n, r) {
  function s(o, u) {
    if (!n[o]) {
      if (!t[o]) {
        var b = o.split("/");
        b = b[b.length - 1];
        if (!t[b]) {
          var a = "function" == typeof __require && __require;
          if (!u && a) return a(b, !0);
          if (i) return i(b, !0);
          throw new Error("Cannot find module '" + o + "'");
        }
        o = b;
      }
      var f = n[o] = {
        exports: {}
      };
      t[o][0].call(f.exports, function(e) {
        var n = t[o][1][e];
        return s(n || e);
      }, f, f.exports, e, t, n, r);
    }
    return n[o].exports;
  }
  var i = "function" == typeof __require && __require;
  for (var o = 0; o < r.length; o++) s(r[o]);
  return s;
}({
  Test: [ function(require, module, exports) {
    "use strict";
    cc._RF.push(module, "6a8f4WjSFlDOp5N76U5Rzru", "Test");
    "use strict";
    var __extends = this && this.__extends || function() {
      var extendStatics = function(d, b) {
        extendStatics = Object.setPrototypeOf || {
          __proto__: []
        } instanceof Array && function(d, b) {
          d.__proto__ = b;
        } || function(d, b) {
          for (var p in b) b.hasOwnProperty(p) && (d[p] = b[p]);
        };
        return extendStatics(d, b);
      };
      return function(d, b) {
        extendStatics(d, b);
        function __() {
          this.constructor = d;
        }
        d.prototype = null === b ? Object.create(b) : (__.prototype = b.prototype, new __());
      };
    }();
    var __decorate = this && this.__decorate || function(decorators, target, key, desc) {
      var c = arguments.length, r = c < 3 ? target : null === desc ? desc = Object.getOwnPropertyDescriptor(target, key) : desc, d;
      if ("object" === typeof Reflect && "function" === typeof Reflect.decorate) r = Reflect.decorate(decorators, target, key, desc); else for (var i = decorators.length - 1; i >= 0; i--) (d = decorators[i]) && (r = (c < 3 ? d(r) : c > 3 ? d(target, key, r) : d(target, key)) || r);
      return c > 3 && r && Object.defineProperty(target, key, r), r;
    };
    Object.defineProperty(exports, "__esModule", {
      value: true
    });
    var _a = cc._decorator, ccclass = _a.ccclass, property = _a.property;
    var TestClass = function(_super) {
      __extends(TestClass, _super);
      function TestClass() {
        var _this = null !== _super && _super.apply(this, arguments) || this;
        _this.background = [];
        _this.prop = null;
        _this.floor = null;
        _this.speedArray = [ 18, 60, 180, 575 ];
        _this.bgWidth = 0;
        _this.floorWidth = 0;
        _this.timeScale = 1;
        return _this;
      }
      TestClass.prototype.start = function() {};
      TestClass.prototype.onLoad = function() {
        for (var _i = 0, _a = this.background; _i < _a.length; _i++) {
          var layer = _a[_i];
          this.bgWidth = 0;
          for (var _b = 0, _c = layer.children; _b < _c.length; _b++) {
            var node = _c[_b];
            node.x = this.bgWidth;
            this.bgWidth += node.width;
          }
        }
        this.floorWidth = 0;
        for (var _d = 0, _e = this.floor.children; _d < _e.length; _d++) {
          var node = _e[_d];
          node.x = this.floorWidth;
          this.floorWidth += node.width;
        }
      };
      TestClass.prototype.update = function(dt) {
        console.log("dt:", dt);
        this.moveBackground(dt);
        this.moveScene(dt);
      };
      TestClass.prototype.moveBackground = function(dt) {
        for (var i = 0, len = this.background.length; i < len; i++) {
          var layer = this.background[i];
          var bgMoveLength = this.speedArray[i] * this.timeScale * dt;
          for (var _i = 0, _a = layer.children; _i < _a.length; _i++) {
            var node_1 = _a[_i];
            node_1.x -= bgMoveLength;
          }
          var node = layer.children[0];
          if (node && node.x <= -node.width) {
            node.x = this.bgWidth + node.x;
            layer.insertChild(node, layer.children.length);
          }
        }
      };
      TestClass.prototype.moveScene = function(dt) {
        var propMoveLength = this.speedArray[3] * this.timeScale * dt;
        for (var i = 0; i < this.prop.children.length; i++) {
          var node = this.prop.children[i];
          node.x -= propMoveLength;
          node && node.x <= -node.width - this.node.width / 2 && (node.x = this.node.width + node.width);
        }
        var num = this.floor.children.length - 1;
        for (var i = 0; i < this.floor.children.length; i++) {
          var node = this.floor.children[i];
          var endNode = this.floor.children[num];
          node.x -= propMoveLength;
          if (node && node.x <= -node.width - this.node.width / 2) {
            node.x = endNode.x + endNode.width;
            this.floor.insertChild(node, this.floor.children.length);
          }
        }
      };
      __decorate([ property(cc.Node) ], TestClass.prototype, "background", void 0);
      __decorate([ property(cc.Node) ], TestClass.prototype, "prop", void 0);
      __decorate([ property(cc.Node) ], TestClass.prototype, "floor", void 0);
      TestClass = __decorate([ ccclass ], TestClass);
      return TestClass;
    }(cc.Component);
    exports.default = TestClass;
    cc._RF.pop();
  }, {} ]
}, {}, [ "Test" ]);