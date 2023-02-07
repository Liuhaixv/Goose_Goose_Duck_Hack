//玩家的userId对应的GameObject
GameObject localPlayerGameObject = GameObject.Find("w8vI5gYsJXcZxkhUqZx4Dg8RNip2");
//获取挂载的LocalPlayer
Handlers.GameHandlers.PlayerHandlers.LocalPlayer localPlayer = localPlayerGameObject.GetComponent<Handlers.GameHandlers.PlayerHandlers.LocalPlayer>();
//放屁
localPlayer.SendFart();