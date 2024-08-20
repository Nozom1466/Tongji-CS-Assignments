const video = document.querySelectorAll(".video");
const videoPlayerList = document.querySelectorAll(".video-playlist");
const videoOnlive = document.querySelector(".video-onlive");
const liveTitle = document.querySelector(".live-title");

let fileName = [
  "Under Scope: Liz and Blue Bird Review",
  "Song Birds MV1",
  "Nozomi Autography",
  " An Open letter to Kyoto Animation",
  "Song Birds MV2",
  "the Third Movement",
  "Liz and Blue Bird Preview",
  "Liz and Blue Bird Snippet",
];

liveTitle.innerHTML = fileName[0];

for (i = 0; i < video.length; i++) {
  (function (n) {
    video[n].onclick = function () {
      for (var j = 0; j < video.length; j++) {
        if (video[j].classList.contains("active")) {
          video[j].classList.remove("active");
        }
      }
      console.log("aaa");
      video[n].classList.add("active");
      console.log(videoOnlive);
      let e = n + 1;
      let videoName = "videos/video" + e + ".mp4";
      videoOnlive.src = videoName;
      liveTitle.innerHTML = fileName[n];
    };
  })(i);
}

const titles = document.querySelectorAll(".video .title");

for (let i = 0; i < fileName.length; i++) {
  titles[i].innerHTML = fileName[i];
}

const durations = document.querySelectorAll(".time");
const listSummary = document.querySelector(".list-summary");
const numericalNumbers = document.querySelectorAll(".numericalNumber");

let timeSum = 0;

for (let i = 1; i <= video.length; i++) {
  let videoName = "videos/video" + i + ".mp4";
  let videoOB = new Audio(videoName);
  numericalNumbers[i - 1].innerHTML = i < 10 ? "0" + i + "." : "" + i + ".";
  videoOB.oncanplay = function () {
    let timeLength = parseInt(videoOB.duration);
    timeSum += timeLength;
    // console.log(timeSum);
    let timeText = "";
    timeText += timeLength / 3600 < 1 ? "" : timeLength / 3600 + ":";
    timeText +=
      (timeLength / 60) % 60 < 1
        ? "0:"
        : parseInt((timeLength / 60) % 60) + ":";
    timeText +=
      timeLength % 60 < 1
        ? "00"
        : timeLength % 60 >= 10
        ? timeLength % 60
        : "0" + (timeLength % 60);
    durations[i - 1].innerHTML = timeText;
  };
}

// 10 lessons &nbsp; . &nbsp; 50m 48s
listSummary.innerHTML = fileName.length + " Episodes";

const darkMode = document.querySelectorAll(".icon-change");
const bodyTag = document.getElementsByTagName("body");

(function () {
  for (let i = 0; i < 2; i++) {
    darkMode[i].onclick = function () {
      if (!bodyTag[0].classList.contains("dark")) {
        bodyTag[0].classList.add("dark");
      } else {
        bodyTag[0].classList.remove("dark");
      }
    };
  }
})();

// 视频
var videoObj = document.querySelector("video");
var playBtn = document.querySelector(".play_pause");
videoObj.muted = false;
videoObj.autoplay = true;
// 时间
var progressTimer = document.querySelector(".progress_timer");
var durationTimer = document.querySelector(".duration_timer");
// 0.实现时间
// 1.获取所需的元素
// 2.时间格式为：hh:mm:ss
// 3.获取总时间，并格式化
// 4.获取当前视频时间，并格式化
// 5.渲染到页面
let { totalT, presentT } = { totalT: 0, presentT: 0 };
videoObj.addEventListener("canplay", function () {
  totalT = this.duration;
  var videoDuration = formatTime(totalT);
  durationTimer.innerHTML = videoDuration;
});

// 进度条
var progress = document.querySelector(".tips-go");
console.log(progress);
// progress.style.width = 0;
// console.log(progress);
//0.实现进度条
//1.获取所需元素
//2.根据时间来实现百分比
var percent = (presentT / totalT) * 100;
progress.style.width = percent + "%";

videoObj.addEventListener("timeupdate", function () {
  presentT = this.currentTime;
  var videoCurrent = formatTime(presentT);
  progressTimer.innerHTML = videoCurrent;
  // console.log(videoCurrent);
  // console.log(presentT);
  //0.实现进度条
  //1.获取所需元素
  //2.根据时间来实现百分比
  var percent = (presentT / totalT) * 100;
  progress.style.width = percent + "%";
});

function formatTime(t) {
  // var h = parseInt(t / 3600);
  // h = h < 10 ? "0" + h : h;
  var m = parseInt(t / 60);
  m = m < 10 ? "0" + m : m;
  var s = parseInt(t % 60);
  s = s < 10 ? "0" + s : s;
  return m + ":" + s;
}

// 0.实现全屏
var expand = document.querySelector(".expand");
expand.addEventListener("click", function () {
  videoObj.webkitRequestFullScreen();
});

let temp = 0;
var v_start = document.querySelector(".v-start");
var v_stop = document.querySelector(".v-stop");
videoObj.addEventListener("click", function () {
  temp = ~temp;
  if (!temp) {
    videoObj.pause();
    v_start.style.display = "block";
    v_stop.style.display = "none";
  } else {
    videoObj.play();
    v_start.style.display = "none";
    v_stop.style.display = "block";
  }
});

v_start.addEventListener("click", function () {
  videoObj.play();
  temp = ~temp;
  v_start.style.display = "none";
  v_stop.style.display = "block";
});

v_start.addEventListener("hover", function () {
  v_start.style.cursor = "pointer";
});

v_stop.addEventListener("click", function () {
  videoObj.pause();
  temp = ~temp;
  v_start.style.display = "block";
  v_stop.style.display = "none";
});

v_stop.addEventListener("hover", function () {
  v_stop.style.cursor = "pointer";
});

// 音量

var vol_input = document.querySelector(".btnsound");
console.log(vol_input);
var vol_length = vol_input.clientWidth;
var vol_processbar = document.querySelector('.input[type="range"]::before');
var event = window.event;

vol_input.onmousedown = function (event) {
  document.onmousemove = function (event) {
    v = vol_input.value;
    document.documentElement.style.setProperty("--vol-len", v + "%");
    videoObj.volume = v / 100;
  };

  document.onmouseup = function (event) {
    event = event || window.event;
    document.onmousemove = null;
    document.onmouseup = null;
  };
};

let speed_list = [0.5, 1.0, 1.5, 2.0];

var speed_buttons = document.querySelectorAll(".speed-button");
for (let i = 0; i < speed_buttons.length; i++) {
  speed_buttons[i].addEventListener("click", function () {
    for (let j = 0; j < speed_buttons.length; j++) {
      if (speed_buttons[j].classList.contains("sp-active")) {
        speed_buttons[j].classList.remove("sp-active");
        break;
      }
    }

    videoObj.playbackRate = speed_list[i];
    speed_buttons[i].classList.add("sp-active");
  });
}

//视频进度条
var main_bar = document.querySelector(".progress-bar");

window.onload = function () {
  videoObj.play();
  videoObj.autoplay = true;
};

main_bar.addEventListener("click", function (e) {
  let perc = e.offsetX / main_bar.clientWidth;
  videoObj.currentTime = parseInt(perc * totalT);
});

main_bar.onmousedown = function (event) {
  document.onmousemove = function (event) {
    let perc = event.offsetX / main_bar.clientWidth;
    videoObj.currentTime = parseInt(perc * totalT);
  };

  // 为右侧圆点绑定鼠标抬起事件
  document.onmouseup = function (event) {
    event = event || window.event;
    // 取消鼠标移动事件
    document.onmousemove = null;
    document.onmouseup = null;
  };
};
