{
  'targets': [
    {
      'target_name': 'cmakebuild',
      'target_type': 'none',
      'actions': [
        {
          'action_name': 'cmakejs',
          'inputs': ['./CMakeLists.txt'],
          'outputs': ['./build'],
          'action': [ 'npm', 'run', 'compilation' ]
        }
      ]
    }
  ]
}
