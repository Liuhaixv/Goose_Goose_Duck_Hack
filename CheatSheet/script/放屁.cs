GameObject localPlayerGameObject = GameObject.Find("w8vI5gYsJXcZxkhUqZx4Dg8RNip2");
Handlers.GameHandlers.PlayerHandlers.LocalPlayer localPlayer = localPlayerGameObject.GetComponent<Handlers.GameHandlers.PlayerHandlers.LocalPlayer>();
Log(localPlayer);
localPlayer.SendFart();