"""Testing for inverse kinematics"""
from math import sqrt
from Python.inverse_kin.inverse_kin import hyp_side_calc, servo_calc, j1_calc, j2_calc, j3_calc, hyp_top_calc

def test_servo_calc_pass():
    """String"""
    result = servo_calc(0,50,70)
    assert result == 0

#Do I need servo calc fail if others have failsafes?

def test_j1_calc():
    """String"""
    result = j1_calc(20,10)
    assert  round(result,2) == 63.43

def test_j1_calc_0():
    """String"""
    result = j1_calc(10,0)
    assert  result == 0

#Do I need j1 fail?

def test_j2_calc_pass():
    """Tests side hypotenuse calculation against precalculated result"""
    result = j2_calc(0,120,-10)
    assert  round(result,2) == 60.94

#Create try catch for the math out of bounds on j2?

def test_j3_calc_pass():
    """Tests j3 calculation against precalculated result"""
    result = j3_calc(0,120,-10)
    assert  round(result,2) == 26.12

#Create try catch for the math out of bounds on j3?

def test_hyp_top_calc_pass():
    """Tests top hypotenuse calculation against precalculated result"""
    result = hyp_top_calc(2,3)
    assert result == sqrt(13)

#Potential to add failure?

def test_hyp_side_calc_pass():
    """Tests side hypotenuse calculation against precalculated result"""
    result = hyp_side_calc(2,3)
    assert  result == sqrt(13)

#Potential to add failure?
