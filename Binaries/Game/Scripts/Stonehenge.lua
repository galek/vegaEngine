function StonehengeStart()
Debug("[LUA]running StonehengeStart"); 
--Skybox
--sky = ActorDynamicSky();
sky = ActorSkyBox("DeferredDemo/SkyBox");
--Meshes
mesh = new ActorMesh("s1.mesh", "s1");
mesh = new ActorMesh("s2.mesh", "s2");
mesh = new ActorMesh("s3.mesh", "s3");
mesh = new ActorMesh("s4.mesh", "s4");
mesh = new ActorMesh("s5.mesh", "s5");
mesh = new ActorMesh("s6.mesh", "s6");
mesh = new ActorMesh("s7.mesh", "s7");
mesh = new ActorMesh("s8.mesh", "s8");
mesh = new ActorMesh("s9.mesh", "s9");
mesh = new ActorMesh("s10.mesh", "s10");
mesh = new ActorMesh("s11.mesh", "s11");
mesh = new ActorMesh("s12.mesh", "s12");
mesh = new ActorMesh("s13.mesh", "s13");
mesh = new ActorMesh("s14.mesh", "s14");
mesh = new ActorMesh("s15.mesh", "s15");
--Camera
camera = new CameraFree(true);
--input
api.SetMouseLock(true);
api.SetBufferedUnBufferedMouseMode(false,false);
end;