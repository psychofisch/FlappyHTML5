//Fischer Thomas gs16m022

//classes
function Vec2()
{
  this.x = 0;
  this.y = 0;
}

function Bird()
{
  this.velocity = 0;
  this.position = new Vec2();
  this.dom = undefined;
  this.isAlive = false;
}

function setPipeSlot(id, slot)
{
  var slots = 10;
  if(slot == undefined)
    slot = Math.floor(Math.random()*slots);
  slot++;

  var margin = 60;
  var pipe = $("#"+id);
  var pipeN = pipe.children(".pipeN");
  var pipeS = pipe.children(".pipeS");
  var gap = 120;//gap is 100px but 50 above and below
  var usable = floor.safezone - gap - margin;
  var positionY = (usable/slots)*slot+(margin*0.5);
  pipeN.css("top", positionY+gap);
  pipeS.css("top", -pipeS.height()+positionY);

  if(debug)
    console.log("pipe '" + id + "' positioned, slot " + slot + " used");

  return pipe;
}

function createPipe(id, slot)
{
  if(debug)
    console.log("pipe '" + id + "' created");
  viewport.append("<div class='hidden pipe' id='"+id+"'><div class='sprite pipeN' id='"+id+"N'></div><div class='sprite pipeS' id='"+id+"S'></div></div>");
  setPipeSlot(id);
  $("#"+id).removeClass("hidden");
  return $("#"+id);
}

function togglePause()
{
  if(debug)
    console.log("PAUSE!");
  gamePause = !gamePause;

  if(gamePause){
    startBtn.show();
  } else {
      startBtn.hide();
  }
}

//some init stuff
var bird = new Bird(),
    gamePause = true,
    viewport,
    floor,
    floors,
    city,
    cities,
    startBtn,
    gameLoopInterval = undefined,
    startFrame = undefined,
    lastFrame = 0,
    deltaTime = 0,
    distance = 0,
    scorefield,
    pipes = [],
    pipeDistance = 52 * 5,
    highscore = 0,
    highscoreField,
    gotInit = false,
    gravity = 1300,
    jumpStrength = 500,
    debug = false,
    god = false;

//handler
$(document).ready(function() {
  init();
});

function init(){
  viewport = $("#viewport");
  viewport.html(
  "<div class='sprite' id='start'></div>"
  +"<div class='' id='flash'></div>"
  +"<div class='' id='scorefield'></div>"
  +"<div class='' id='highscorefield'></div>"
  +"<div class='sprite birdFly' id='bird'></div>"
  +"<div id='city0' class='sprite city'></div>"
  +"<div id='floor0' class='sprite floor'></div>"
  );

  //floors
  floor = $("#floor0");
  floor.safezone = viewport.height() - floor.height();
  floor.css("top", floor.safezone);

  var floorNumber = Math.ceil(viewport.width()/floor.width());
  for(var i = 0; i < floorNumber; i++)
  {
    if(debug)
      console.log("floor"+i+" created!");
    viewport.append("<div id='floor"+(i+1)+"' class='sprite floor'></div>");
  }

  floors = $(".floor");
  floors.each(function(i){
    floors[i].position = floor.width() * i;
    $(this).css("left", floors[i].position);
    $(this).css("top", floor.safezone);
  });

  //cities
  city = $("#city0");
  city.yStart = floor.safezone - city.height();
  city.css("top", city.yStart);

  var cityNumber = Math.ceil(viewport.width()/city.width());
  for(var i = 0; i < cityNumber; i++)
  {
    if(debug)
      console.log("city"+i+" created!");
    viewport.append("<div id='city"+(i+1)+"' class='sprite city'></div>");
  }

  cities = $(".city");
  cities.each(function(i){
    cities[i].position = city.width() * i;
    $(this).css("left", cities[i].position);
    $(this).css("top", city.yStart);
  });

  //inits
  bird.dom = $("#bird");
  startBtn = $("#start");
  scorefield = $("#scorefield");
  highscorefield = $("#highscorefield");

  startBtn.css({
    "top": viewport.height() * 0.5 - 28,
    "left": viewport.width() * 0.5 - 40
  });

  bird.position.x = viewport.width() * 0.3;
  bird.dom.css("left", bird.position.x);
  bird.position.y = viewport.height() * 0.5;
  bird.dom.css("top", bird.position.y);

  scorefield.append("<div class='sprite numberB' id='score2'></div>");
  scorefield.append("<div class='sprite numberB' id='score1'></div>");
  scorefield.append("<div class='sprite numberB' id='score0'></div>");
  scorefield.css("left", (viewport.width()-scorefield.width())*0.5);

  highscorefield.append("<div class='sprite numberS' id='hscore2'></div>");
  highscorefield.append("<div class='sprite numberS' id='hscore1'></div>");
  highscorefield.append("<div class='sprite numberS' id='hscore0'></div>");
  highscorefield.css("left", (viewport.width()-highscorefield.width())*0.5);
  highscorefield.css("top", scorefield.position().top + 25);

  startBtn.on("click", startGame);

  if(gotInit == false)
  {
    $(document).keydown(function(event)
    {
      if(debug)
        console.log(event);
      switch(event.key)
      {
        case "Escape":
        if(gamePause == false)
          togglePause();
        break;
      }
    });
  }

  gotInit = true;
}

//logic
function startGame()
{
  if(debug)
    $("#debug").show();
  else {
    $("#debug").hide();
  }

  viewport.off("click");
  viewport.click("click", click);
  $("#flash").removeClass("flashStart");

  bird.dom.addClass("birdFly");
  bird.position.y = viewport.height() * 0.5;
  bird.velocity = 0;

  var pipeNumber = Math.ceil(viewport.width() / pipeDistance);
  for(var i = 0; i < pipeNumber; i++)
  {
    if(pipes.length < pipeNumber)
    {
      pipes[i] = {};
      pipes[i].dom = createPipe("pipe"+i);
    }

    setPipeSlot("pipe"+i);
    pipes[i].position = viewport.width() + (pipeDistance*i);
  }

  distance = bird.position.x - pipes[0].position;
  //hard mode
  distance -= bird.dom.width();

  scorefield.children("#score0").css("background-position-x", "");
  scorefield.children("#score1").css("background-position-x", "");
  scorefield.children("#score2").css("background-position-x", "");

  var score = Math.max(0, Math.ceil((highscore)/pipeDistance));
  var strLen = score.toString().length;
  for(var i = 0; i < strLen; i++)//only works as long as highscore/score is below 1000
  {
    var scoreCurrent = score.toString()[strLen-(i+1)];
    highscorefield.children("#hscore"+i).css("background-position-x", (scoreCurrent)*-14);
  }

  bird.isAlive = true;

  togglePause();

  window.requestAnimationFrame(gameLoop);
}

function click()
{
  if(!gamePause)
  {
    bird.velocity = -jumpStrength;
    if(debug)
      console.log("Jump!");
  }
  else {
    if(debug)
      console.log("Put Your Hands Up In The Air!");
  }
}

function gameLoop(timeAlive)
{
  if(gamePause)
    return;

  if(startFrame == undefined)
  {
    startFrame = timeAlive;
  }

  timeAlive -= startFrame;
  deltaTime = (timeAlive - lastFrame)*0.001;//ms to s conversion
  lastFrame = timeAlive;

  if(deltaTime > 0.1) //safety precaution
    deltaTime = 0.1;

  //move background
  var cityMove = 50*deltaTime;
  cities.each(function(i){
    cities[i].position -= cityMove;
    $(this).css("left", cities[i].position);
  });

  if(cities[0].position + city.width() < 0)
  {
    cities.each(function(i){
      cities[i].position = city.width() * i;
    });
  }

  //move active elements
  var floorMove = 300*deltaTime;
  floors.each(function(i){
    floors[i].position -= floorMove;
    $(this).css("left", floors[i].position);
  });

  if(floors[0].position + floor.width() < 0)
  {
    floors.each(function(i){
      floors[i].position = floor.width() * i;
    });
  }

  for(var i = 0; i < pipes.length; i++)
  {
    pipes[i].position -= floorMove;
    pipes[i].dom.css("left", pipes[i].position);

    if(pipes[i].position+52 < 0)
    {
      pipes[i].position = pipeDistance * pipes.length - 52;
      setPipeSlot("pipe"+i);
    }
  }

  var birdRotation = jumpStrength*(bird.velocity/10000);
  bird.dom.css("transform", "rotate(" + birdRotation + "deg)");

  //physics
  //if(bird.velocity.y <= 1000) //nevermind...
    bird.velocity += gravity*deltaTime;

  //logic
  distance += floorMove;

  var birdRight = bird.position.x + bird.dom.width();
  var pipeWidth = pipes[0].dom.children(0).width();//all pipes are the same and at least one exists
  var pipeHeight = pipes[0].dom.children(".pipeS").height();
  for(var i = 0; i < pipes.length; i++)
  {
    if(bird.isAlive == true
      && bird.position.x > pipes[i].position
      && birdRight < pipes[i].position + pipeWidth
      && (bird.position.y > pipes[i].dom.children(".pipeN").position().top
      || bird.position.y < pipes[i].dom.children(".pipeS").position().top + pipeHeight)
      && !god)
    {
      bird.isAlive = false;
      if(debug)
        console.log("bomp");
    }
  }

  if(bird.isAlive == false || bird.position.y + bird.dom.height() >= floor.safezone)
  {
    $("#flash").addClass("flashStart");
    bird.dom.removeClass("birdFly");
    //bird.position.y -= bird.velocity; //does look weird sometimes
    bird.isAlive = false;
    if(distance > highscore)
    {
      highscore = distance;
      $("#debug").html(highscore);
    }
    togglePause();
  }

  bird.position.y += bird.velocity * deltaTime;
  bird.dom.css("top", bird.position.y);

  //Score
  var score = Math.max(0, Math.ceil((distance)/pipeDistance));
  $("#debug").html(score);
  var strLen = score.toString().length;
  for(var i = 0; i < strLen; i++)
  {
    var scoreCurrent = score.toString()[strLen-(i+1)];
    scorefield.children("#score"+i).css("background-position-x", (scoreCurrent)*-16);
  }

  window.requestAnimationFrame(gameLoop);
}
