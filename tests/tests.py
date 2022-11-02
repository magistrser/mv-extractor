import os
import unittest
import time
import numpy as np

from mvextractor.videocap import VideoCap


video_url = os.getenv('VIDEO_URL', 'vid_h264.mp4')


def validate_timestamp(timestamp, tolerance=10.0):
    assert isinstance(timestamp, float)
    assert timestamp <= time.time() <= timestamp+tolerance


def validate_frame(frame):
    assert type(frame) == np.ndarray
    assert frame.dtype == np.uint8
    assert frame.shape == (720, 1280, 3)


def validate_motion_vectors(motion_vectors, shape=(0, 10)):
    assert type(motion_vectors) == np.ndarray
    assert motion_vectors.dtype == np.int32
    assert motion_vectors.shape == shape


class TestMotionVectorExtraction(unittest.TestCase):

    # run before every test
    def setUp(self):
        self.cap = VideoCap()


    # run after every test regardless of success
    def tearDown(self):
        self.cap.release()


    def open_video(self):
        ret = self.cap.open(video_url)
        assert ret == True


    def test_init_cap(self):
        self.cap = VideoCap()
    

    def test_open_video(self):
        self.open_video()

    
    def test_open_invalid_video(self):
        ret = self.cap.open("vid_not_existent.mp4")
        assert ret == False


    def test_read_not_opened_cap(self):
        ret = self.cap.open("vid_not_existent.mp4")
        assert ret == False
        ret, frame, frame_type, timestamp = self.cap.read()
        assert frame_type == "?"
        assert timestamp == 0.0
        assert ret == False
        assert frame == None


    def test_read_first_I_frame(self):
        self.open_video()
        ret, frame, frame_type, timestamp = self.cap.read()
        assert ret == True
        assert frame_type == "I"
        validate_timestamp(timestamp)        
        validate_frame(frame)


    def test_read_first_P_frame(self):
        self.open_video()
        self.cap.read()  # skip first frame (I frame)
        ret, frame, frame_type, timestamp = self.cap.read()
        assert ret == True
        assert frame_type == "P"
        validate_timestamp(timestamp)        
        validate_frame(frame)


    def test_read_first_ten_frames(self):
        rets = []
        frames = []
        motion_vectors = []
        frame_types = []
        timestamps = []
        self.open_video()
        for _ in range(10):
            ret, frame, frame_type, timestamp = self.cap.read()
            rets.append(ret)
            frames.append(frame)
            frame_types.append(frame_type)
            timestamps.append(timestamp)

        assert all(rets)
        assert frame_types == ['I', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P']
        [validate_timestamp(timestamp) for timestamp in timestamps]
        [validate_frame(frame) for frame in frames]
        shapes = [
            (0, 10), (3665, 10), (3696, 10), (3722, 10), (3807, 10), 
            (3953, 10), (4155, 10), (3617, 10), (4115, 10), (4192, 10)
        ]


    def test_frame_count(self):
        self.open_video()
        frame_count = 0
        while True:
            ret, _, _, _ = self.cap.read()
            if not ret:
                break
            frame_count += 1
        assert frame_count == 337


    def test_timings(self):
        self.open_video()
        times = []
        while True:
            tstart = time.perf_counter()
            ret, _, _, _ = self.cap.read()
            if not ret:
                break
            tend = time.perf_counter()
            telapsed = tend - tstart
            times.append(telapsed)
        print(f"Timings: mean {np.mean(times)} -- std: {np.std(times)}")



if __name__ == '__main__':
    unittest.main()
